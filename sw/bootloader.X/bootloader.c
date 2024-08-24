#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/nvm/nvm.h"
#include "bootloader.h"
#include "serial.h"

//
// TRS HARD DRIVE emulator bootloader
//
// - bootloader lives in 0000-0FFF
// - Enter bootloader by erasing the last two locations of the EEPROM (0x3FE-3FF),
//   and reset the PIC.
// - In bootloader mode, the TRS80 can only write data to the interface.
//   The data is sent in intel-hex format. TRS I/O address 0xC0
//   Once a line is sent, write any data to address 0xC1. This will trigger the
//   processing of the line. During processing, the TRS80 can poll the status
//   register, and wait to the READY bit to be set. The lower nibble of status 
//   indicates the result of the processing (0 = no error)
// - The hex file must contain the CRC16 of the image (0x1000 -> 0x1FFFD) at
//   location 1FFFE-1FFFF. If the CRC is invalid, the code will remain in
//   bootloader.
// - The last 2 bytes of the EEPROM (3FE-3FF) should also contain the CRC. If
//   the CRC is not found, the PIC will enter bootloader, but a timer will 
//   start. After ~30 seconds, the bootloader will rewrite the CRC at the end
//   of the EEPROM (if the flash is valid, obviously) and exit bootloader.
// - An additional type (06) has been added in the intel-hex format. It is used
//   to erase flash (use address FF00) or eeprom (use address FF01)
//

//
// 18F47Q83
//
#define BLOCKSIZE		0x100
#define ERASE_BLOCKSIZE	0x100

//
// local defines
//
#define TRS_ADDR		PORTA
#define TRS_A0			PORTAbits.RA3
#define TRS_A1			PORTAbits.RA2
#define TRS_A2			PORTAbits.RA1
#define TRS_A3			PORTAbits.RA0
#define TRS_DATA_IN		PORTD
#define TRS_DATA_OUT	LATD
#define TRS_DATA_TRIS	TRISD
#define TRS_RD_N		PORTAbits.RA4
#define TRS_WAIT		LATBbits.LATB3
#define TRS_WAIT_TRIS	TRISBbits.TRISB3
#define GAL_INT_IE		PIE6bits.INT1IE
#define GAL_INT_IP		IPR6bits.INT1IP
#define GAL_INT_IF		PIR6bits.INT1IF
#define GAL_INT_EDGE	INTCON0bits.INT1EDG
#define GAL_INT			PORTBbits.RB1
#define STAT_CS			LATCbits.LATC0
#define STAT_CS_TRIS	TRISCbits.TRISC0
#define SPI_SCK_TRIS	TRISCbits.TRISC3
#define SPI_MOSI_TRIS	TRISCbits.TRISC5

#define GLED			LATBbits,LATB7
#define GLED_TRIS		TRISBbits,TRISB7
#define RLED			LATBbits,LATB6
#define RLED_TRIS		TRISBbits,TRISB6

// hard drive emulator status register. Lower nibble = errno
#define TRS_BOOT_BUSY	0x80
#define TRS_BOOT_READY	0x40

#define TRS_BOOT_OK			0x00
#define TRS_BOOT_RANGE_ERR	0x01
#define TRS_BOOT_IGNORED  	0x02
#define TRS_BOOT_CHECKSUM_ERR	0x03
#define TRS_BOOT_INVALID 	0x04
#define TRS_BOOT_SEQ_ERR	0x05
#define TRS_BOOT_PROTECTED	0x06

// flags
#define BF_PROCESS			1	// true if a line should be processed
#define BF_IGNORE			2	// true if everything should be ignored
#define BF_DIRTY			3	// true if holding regs are dirty
#define BF_FLASH_INVALID	6	// true if flash has incorrect CRC16
#define BF_EEPROM_INVALID	7	// true if eeprom has incorrect CRC16

#define INPUT_PIN 		1
#define OUTPUT_PIN 		0
#define SD_CS			LATAbits.LATA5		// SD chip select
#define SPI_MISO_TRIS	TRISCbits.TRISC4	// SPI Master input/Slave output TRIS
#define SPI_MOSI_TRIS	TRISCbits.TRISC5	// SPI Master output/Slave input TRIS
#define TRS_ADDR_TRIS	TRISA

const uint16_t	boot_version __at(0x6) = (BOOTLOADER_MINOR << 8) | BOOTLOADER_MAJOR;

//
// DATA
//
//var_g	UDATA_ACS 0x7B
__near volatile	uint8_t		timeout;
__near __persistent	uint16_t	crc;
__near __persistent	volatile	uint8_t		flags;

//var_b0	UDATA_OVR 0x80
__near uint8_t		checksum;
__near uint8_t		count;
__near uint32_t	addr;
__near uint8_t		type;

// buffers
uint8_t BUFX[256];
__near uint8_t fsr2;
__near uint24_t tblptr;


void b_process(void);
void update_status(uint8_t status);


//	
// low priority interrupt
//
// - if not in bootloader mode, go to app lo interrupt routine
// - ignore TRS reads
// - receive data in *FSR2
// - if A0 is set, set the process flag and BUSY status
//
void __interrupt(low_priority) boot_int_lo(void)
{
    if(GAL_INT_IF) // GAL interrupt?
    {
        GAL_INT_IF = 0; // ack interrupt
        if(TRS_RD_N)
        {
            BUFX[fsr2++] = TRS_DATA_IN; // put one byte in buffer
            if(TRS_A0)
            {
                flags |= 1 << BF_PROCESS; // set flag for main loop
                update_status(TRS_BOOT_BUSY); // update status with BUSY bit
            }
        }
        else
        {
            // bi_read
            //uint8_t trs_addr = TRS_ADDR & 0x0F;
            if((TRS_ADDR & 0x0F) == 0) // C0 -> major version
            {
                TRS_DATA_OUT = boot_version >> 8;
            }
            else if((TRS_ADDR & 0x0F) == 1) // C1 -> minor version
            {
                TRS_DATA_OUT = (uint8_t)boot_version;
            }
            else if((TRS_ADDR & 0x0F) == 2) // C2 -> CRC high
            {
                TRS_DATA_OUT = crc >> 8;
            }
            else if((TRS_ADDR & 0x0F) == 3) // C3 -> CRC low
            {
                TRS_DATA_OUT = (uint8_t)crc;
            }
            else // others -> magic 4F
            {
                TRS_DATA_OUT = 0x4F;
            }
            TRS_DATA_TRIS = 0;
        }
        TRS_WAIT = 1; // tell GAL to release TRS_WAIT
        while(GAL_INT == 0) ;
        TRS_DATA_TRIS = 255;
        TRS_WAIT = 0; // be ready for next transfer
    }
    if(PIE3bits.TMR0IE == 1 && PIR3bits.TMR0IF == 1)
    {
        PIR3bits.TMR0IF = 0;
        if(timeout)
            timeout--;
    }
    if(PIE4bits.U1TXIE && PIR4bits.U1TXIF) {
        //PIR4bits.U1TXIF = 0;
		serial_tx_int();
	}
    if(PIE4bits.U1RXIE && PIR4bits.U1RXIF) {
        //PIR4bits.U1RXIF = 0;
		serial_rx_int();
	}
}

//
// update status (with SPI)
//
void update_status(uint8_t status)
{
    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
    SPI1TCNTL = 1;
    SPI1TXB = status;
    while(!PIR3bits.SPI1RXIF) ;
    STAT_CS = 1;
    SPI1RXB;
    STAT_CS = 0;
}

//
// read byte : take 2 ascii chars (pointed by FSR2) and convert
//
uint8_t read_byte(void)
{
    uint8_t hi, lo;
    hi = BUFX[fsr2++] - '0';
    if(!(hi < 10))
        hi -= 'A' - '0' - 10;
    lo = BUFX[fsr2++] - '0';
    if(!(lo < 10))
        lo -= 'A' - '0' - 10;
    return (uint8_t)((hi << 4) | lo);
}


void pic_init(void)
{
	/* SD Card */
	SD_CS_TRIS = OUTPUT_PIN;
	SD_WP_TRIS = INPUT_PIN;
	SD_CD_TRIS = INPUT_PIN;
	SPI_SCK_TRIS = OUTPUT_PIN;
	SPI_MISO_TRIS = INPUT_PIN;
	SPI_MOSI_TRIS = OUTPUT_PIN;
	SD_CS = 1;

	/* Status 74HC595 */
	STAT_CS_TRIS = OUTPUT_PIN;
	STAT_CS = 0;

	/* TRS80 and GAL */
//	ADCON1 = 0x0f;
	TRS_ADDR_TRIS |= 0x0F;
	TRS_WAIT_TRIS = OUTPUT_PIN;
	TRS_WAIT = 0;
	TRS_RD_N_TRIS = INPUT_PIN;
	GAL_INT_TRIS = INPUT_PIN;
	GAL_INT_EDGE = 0;	// interrupt on falling edge
	GAL_INT_IP = 0;
	GAL_INT_IF = 0;
	GAL_INT_IE = 1;

	/* DS1307 interrupt */
	RTC_INT_TRIS = INPUT_PIN;

	/* Debug LED */
	GLED_TRIS = OUTPUT_PIN;
	RLED_TRIS = OUTPUT_PIN;
    GLED = 1; // turn both LEDs on
    RLED = 1;

	/* Timer 0 : ~1Hz */
//	OpenTimer0(TIMER_INT_ON & T0_PS_1_256 & T2_POST_1_1);
//	PR0 = 65535;
	timeout = 32; // about 30 seconds (32 * 16e6 / 256 / 65536)
	IPR3bits.TMR0IP = 0;

	/* Enable SPI */
	SPI1CON0 = 0x02;
	SPI1CON1 = 0x40;
	SPI1CON2 = 0x00;
#if 1 // 64MHz
	SPI1BAUD = 0x02; // fast clock
#else // 40MHz
	SPI1BAUD = 0x01; // fast clock
#endif
	SPI1CLK = 0x00;
	SPI1CON0bits.EN = 1;

	/* Interrupt priorities */
	INTCON0bits.IPEN = 1;
}


void main(void)
{
	SYSTEM_Initialize();
	pic_init();
	usart_init();

    addr = 0; // init our variables
    fsr2 = 0;

    INTCON0bits.GIEL = 1; // enable interrupts
    INTCON0bits.GIE = 1;

	TRS_WAIT = 0; // assert wait trigger

    update_status(TRS_BOOT_READY); // we are ready !
    NVM_UnlockKeySet(UNLOCK_KEY);

    while((flags & (1 << BF_FLASH_INVALID)) // flash valid ?
       || timeout) // timeout ?
    {
        if(flags & (1 << BF_PROCESS)) // work to do ?
            b_process();
    }

    // exit_bootloader
    // restore eeprom CRC
    EEPROM_Write((uint24_t)EEPROM_START_ADDRESS + (uint24_t)EEPROM_CRC_ADDR, crc);
    while(NVM_IsBusy()) ;
    EEPROM_Write((uint24_t)EEPROM_START_ADDRESS + (uint24_t)(EEPROM_CRC_ADDR + 1), crc >> 8);
    while(NVM_IsBusy()) ;
    NVM_UnlockKeyClear();
    asm("reset");
}

void erase_flash(void);
void erase_eeprom(void);
void b_last(void);
void b_write_block(void);
void do_write(void);

//
// process one line of the intel file
//	
void b_process(void)
{
    uint8_t status = TRS_BOOT_OK;

    flags &= ~(1 << BF_PROCESS);

    BUFX[fsr2] = 0;
    usart_puts2_r(BUFX);
    //
    // step 1 : confirm line starts with ':'
    //
    fsr2 = 0;
    if(BUFX[fsr2++] == ':') // first char must be ':'
    {
        //
        // step 2 : verify checksum
        //
        count = read_byte(); // number of data bytes
        checksum = count; // init checksum
        count += 4; // addr hi/lo, type, checksum
        while(count--)
            checksum += read_byte();
        if(checksum == 0) // checksum must be 0
        {
            //
            // step 3 : evaluate line
            //
            fsr2 = 1;
            count = read_byte(); // number of data bytes
            addr &= 0xFFFF0000;
            addr |= (uint32_t)read_byte() << 8; // addr hi
            addr |= (uint32_t)read_byte(); // addr lo
            type = read_byte(); // type
            switch(type & 7)
            {
            case 0: // 00 = regular data
                //
                // TYPE 00 : data
                //
                // b_data
                if(!(flags & (1 << BF_IGNORE)))
                {
                    if(((addr >> 24) & 0xFF) == 0x00) // check addr+3 : must be 0
                    {
                        // dispatch write based on addr+2
                        if(((addr >> 16) & 0xFE) == 0x00) // 00,01 => main flash
                        {
                            // write some data in flash
                            // b_data_flash
                            if(tblptr <= (uint24_t)addr) // TBLPTR > load => sequence error
                            {
                                while(count)
                                {
                                    if(tblptr < (uint24_t)addr) // TBLPTR < load => fill with FF
                                    {
                                        //*(__rom uint8_t *)tblptr++ = 0xFF;
                                        *(uint8_t *)(BUFFER_RAM_START_ADDRESS + (tblptr++ & (BLOCKSIZE - 1))) = 0xFF;
                                    }
                                    else
                                    {
                                        //*(__rom uint8_t *)tblptr++ = read_byte();
                                        *(uint8_t *)(BUFFER_RAM_START_ADDRESS + (tblptr++ & (BLOCKSIZE - 1))) = read_byte();
                                        count--;
                                        flags |= (1 << BF_DIRTY);
                                        addr++;
                                    }

                                    if((tblptr & (BLOCKSIZE - 1)) == 0)
                                        b_write_block();
                                }
                            }
                            else
                            {
                                flags |= 1 << BF_IGNORE; // ignore everything
                                status = TRS_BOOT_SEQ_ERR;
                            }
                        }
                        else
                        {
                            b_last(); // not 00,01 => make sure last block is done

                            if(((addr >> 16) & 0xFF) == (EEPROM_START_ADDRESS >> 16)) // 38 => eeprom
                            {
                                // write some data in eeprom
                                // b_data_eeprom
                                usart_put_short(addr); usart_send('-'); usart_put_short(addr + count - 1); usart_send('\r'); usart_send('\n');

                                if(((addr >> 8) & 0xFC) == 0) // upper 6 bits of addr+1 must be 0
                                {
                                    while(count)
                                    {
                                        EEPROM_Write((uint24_t)addr++, read_byte());
                                        while(NVM_IsBusy()) ;
                                        count--;
                                    }
                                }
                                else
                                    status = TRS_BOOT_RANGE_ERR;
                            }
                            else
                                status = TRS_BOOT_IGNORED; // everything else => ignored
                        }
                    }
                    else
                        status = TRS_BOOT_IGNORED;
                }
                else
                    status = TRS_BOOT_IGNORED;
                break;

            case 1: // 01 = end of file
                b_last();
                NVM_UnlockKeyClear();

                //INTCON0bits.GIE = 0; // disable interrupts
                //INTCON0bits.GIEL = 0;
                //asm("goto " ___mkstr(APP_STARTUP));
                asm("reset");
                break;

            case 4: // 04 = set address upper 16 bits
                // 
                // TYPE 04 : set upper 16 bits of address
                //   address high :
                //   0020 : ID location
                //   0030 : CONFIG location
                //   0038 : EEPROM location
                //
                // b_set_hi_addr
                if(count == 2) // make sure block length is 2
                {
                    addr &= 0x0000FFFF;
                    addr |= (uint32_t)read_byte() << 24;
                    addr |= (uint32_t)read_byte() << 16;
                }
                else
                    status = TRS_BOOT_INVALID;
                break;

            case 6: // 06 = erase
                //
                // Type 06 - erase. Address must be FF0x (x=0 for flash, x=1 for eeprom)
                //
                if(((addr >> 8) & 0xFF) == 0xFF)
                {
                    if((addr & 0xFF) == 0)
                        erase_flash();
                    else
                        erase_eeprom();
                }
                else
                    status = TRS_BOOT_INVALID;
                break;

            default:
                status = TRS_BOOT_IGNORED;
                break;
            }
        }
        else
            status = TRS_BOOT_CHECKSUM_ERR;
    }
    else
        status = TRS_BOOT_INVALID;

    fsr2 = 0; // get ready for next line
    update_status(TRS_BOOT_READY | status); // update status
}

//
// erase flash
//
void erase_flash(void)
{
    usart_puts2_r("Erasing Flash");

    flags |= 1 << BF_FLASH_INVALID;

    for(tblptr = (uint24_t)(BOOTLOADER_END+1); tblptr < (uint24_t)FLASH_CRC_ADDR; tblptr += (uint24_t)ERASE_BLOCKSIZE)
        FLASH_PageErase(tblptr);

    tblptr = 0;
}

//
// erase eeprom 000-3FF
//
void erase_eeprom(void)
{
    uint16_t addr;
    usart_puts2_r("Erasing EEPROM");

    for(addr = 0; addr < EEPROM_SIZE; addr++)
    {
        EEPROM_Write((uint24_t)EEPROM_START_ADDRESS + (uint24_t)addr, 0xFF);
        while(NVM_IsBusy()) ;
    }
}

// commit eventual last block
void b_last(void)
{
    if(flags & (1 << BF_DIRTY)) // something to write ?
    {
        while((tblptr & (BLOCKSIZE - 1)) != 0)
        {
            *(uint8_t *)(BUFFER_RAM_START_ADDRESS + (tblptr++ & (BLOCKSIZE - 1))) = 0xFF;
        }
        b_write_block();
    }
}

//
// write block subroutine
//
void b_write_block(void)
{
    if(flags & (1 << BF_DIRTY)) // any data to write ?
    {
        flags &= ~(1 << BF_DIRTY);
        tblptr--;
        if(!(tblptr < (uint24_t)(BOOTLOADER_END + 1)))
        {
            // b_write_addr_ok
            usart_put_long(tblptr & ~(BLOCKSIZE - 1)); usart_send('-'); usart_put_long(tblptr); usart_send('\r'); usart_send('\n');

            flags |= 1 << BF_FLASH_INVALID;
//            do_write();
            FLASH_RowWrite(tblptr & ~(BLOCKSIZE - 1), (flash_data_t *)BUFFER_RAM_START_ADDRESS);
        }
        tblptr++;
    }
}

//
// write magic sequence
//
void do_write(void)
{
    //Load NVMADR with the start address of the memory page
    NVMADR = tblptr - 1;

    //Set the page write command
    NVMCON1bits.NVMCMD = 0x05;

    //Disable global interrupt
    INTCON0bits.GIE = 0;

    //Perform the unlock sequence   
    asm("asmopt push"); //save the current selection of optimizations
    asm("asmopt off"); //turn off assembler optimizations
    asm("banksel(_NVMLOCK)"); //select the bank of the NVMLOCK register
    NVMLOCK = 0x55; //assign 'unlockKeyLow' to NVMLOCK.
    asm("MOVF 0xAA,w,c"); //load 'unlockKeyHigh' into the W register
    asm("MOVWF (_NVMLOCK&0xFF),b"); //move the W register to NVMLOCK

    //Start page write 
    asm("bsf (_NVMCON0bits&0xFF)," ___mkstr(_NVMCON0_GO_POSN) ",b"); //Set GO bit  
    asm("asmopt pop"); //restore assembler optimization settings

    //Restore global interrupt enable bit value
    INTCON0bits.GIE = 1;

    //Clear the NVM Command
    NVMCON1bits.NVMCMD = 0x00;
}
