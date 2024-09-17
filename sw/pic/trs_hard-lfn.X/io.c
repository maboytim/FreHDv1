#include <xc.h>
#include "led.h"
#include "integer.h"
#include "FatFS/ff.h"
#include "FatFS/diskio.h"
#include "trs_hard.h"

/* Global variables */
#pragma udata fs_buffer
extern FATFS fs;
extern Drive state_d[TRS_HARD_MAXDRIVES];
extern FIL state_file2;
#if EXTRA_IM_SUPPORT
#if _USE_FASTSEEK
DWORD im_stbl[FAST_SEEK_LEN];
#endif
extern FIL im_file;
extern BYTE im_buf[0x80];
#endif
BYTE sector_buffer[MAX_SECTOR_SIZE];
#if EXTRA_IM_SUPPORT
extern image_t im[8];
#endif
#pragma udata extra_buffer
extern BYTE extra_buffer[EXTRA_SIZE];
#pragma udata
extern DIR state_dir;
extern FILINFO state_fno;
extern UCHAR state_dir_open;
extern UCHAR state_file2_open;
extern UCHAR fs_mounted;
extern UCHAR cur_unit;
extern UCHAR led_count;
extern led_t rled;
extern led_t gled;

extern FATFS fs;

extern __near BYTE action_flags;
extern __near BYTE action_type;
extern __near BYTE action_status;
extern __near UCHAR state_busy;
extern __near UCHAR state_status;
extern __near UCHAR state_present;
extern __near UCHAR state_control;
extern __near UCHAR state_error;
extern __near UCHAR state_seccnt;
extern __near UCHAR state_secnum;
extern __near USHORT state_cyl;
extern __near UCHAR state_drive;
extern __near UCHAR state_head;
extern __near UCHAR state_wp;
extern __near UCHAR state_command;
extern __near USHORT state_bytesdone;
extern __near UCHAR state_secsize;
extern __near USHORT state_secsize16;
extern __near UCHAR state_command2;
extern __near UCHAR state_error2;
extern __near USHORT state_size2;
extern __near UCHAR state_bytesdone2;
extern __near UCHAR state_romdone;
extern __near UCHAR state_rom;
extern __near UCHAR crc7;
extern __near USHORT crc16;
extern __near UCHAR expected_addr;
extern __near UCHAR data2_out, data2_out1;
extern __near UCHAR data2_stream;
extern __near UCHAR foo;

extern const BYTE loader_code[256];


#define TRS_ADDR        PORTA
#define TRS_DATA_IN     PORTD
#define TRS_DATA_OUT    LATD
#define TRS_DATA_TRIS   TRISD
#define GAL_INT_IF      PIR6bits.INT1IF
#define GAL_INT_IE      PIE6bits.INT1IE
#define GAL_INT         PORTBbits.RB1
#define TRS_WAIT        LATBbits.LATB3
#define STAT_CS         LATCbits.LATC0
#define GLED            LATBbits.LATB7
#define RLED            LATBbits.LATB6

// high priority interrupt
void __interrupt() handle_int2(void)
{
    if(TRS_ADDR == expected_addr) // are we expecting this?
    {
        TRS_DATA_TRIS = 0; // TRS_DATA was set, let it go
        // release wait
        TRS_WAIT = 1;
        // wait for bus cycle to finish
        while(GAL_INT == 0) ;
        TRS_DATA_TRIS = 0xFF;
        // reassert wait trigger
        TRS_WAIT = 0;

        PIR6bits.INT1IF = 0; // acknowledge the interrupt

        while(!PIR3bits.SPI1RXIF) ;
        __nop();
        TRS_DATA_OUT = data2_out = SPI1RXB;

        // start next read
        //SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
        SPI1TCNTL = 1;
        SPI1TXB = 255;

        gled.val |= LED_FLASH; // (indicate streaming)
    }
    else
    {
    UCHAR new_status = 0;

    PIR6bits.INT1IF = 0; // acknowledge the interrupt

    if(!state_busy) // state_busy should always be 0 (unless TRS forgot to poll STATUS)
    {
        // if streaming there is a spi transfer in flight that needs to finish
        if(expected_addr != 0xff)
        {
            while(!PIR3bits.SPI1RXIF) ;
            __nop();
            data2_out1 = SPI1RXB;
            LATAbits.LATA5 = 1; // SD_CS = 1; // deassert the SD CS

            expected_addr = 0xff; // invalidate fast case
        }

        if(!(TRS_ADDR & 0x10)) // TRS read (0) or write (1) ?
        {
            // read
            switch(TRS_ADDR & 0x0F)
            {
            case 0x0: // 0 R C0 192
                // trs_read_wp
                TRS_DATA_OUT = state_wp;
                break;
            case 0x1: // 1 R C8 200
                // trs_read_data
                TRS_DATA_OUT = sector_buffer[state_bytesdone++];
                if(state_bytesdone == state_secsize16)
                {
                    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                    SPI1TCNTL = 1;
                    SPI1TXB = new_status = (TRS_HARD_READY | TRS_HARD_SEEKDONE); // done !
                    state_bytesdone = 0;
                }
                break;
            case 0x2: // 2 R C4 196
                // trs_read_rom
#if 0 // rom in ee
                // 2nd byte (going to RAM) ?
                if(state_romdone == 0x02)
                {
                    TRS_DATA_OUT = state_rom;
                }
                else
                {
                    // Load NVMADR with the desired byte address
                    NVMADRL = state_romdone;
                    NVMADRH = 0x00;
                    NVMADRU = 0x38;
                    NVMCON1bits.CMD = 0x00; // Set the byte read command
                    NVMCON0bits.GO = 1; // Start byte read
                    //while (NVMCON0bits.GO); // Wait for the read operation to complete
                    TRS_DATA_OUT = NVMDATL; // Store the read value to a variable
                }
#else
                // 2nd byte (going to RAM) ?
                TRS_DATA_OUT = (state_romdone == 0x02 ? state_rom : loader_code[state_romdone]);
#endif
                state_romdone++;
                break;
            case 0x3: // 3 R CC 204 
                // trs_read_cyllo
                TRS_DATA_OUT = state_cyl;
                break;
            case 0x4: // 4 R C2 194
                // trs_read_data2_slow
                // Unanticipated read from data2.
                if(data2_stream)
                {
                    expected_addr = TRS_ADDR & 0x1F; // ensure expected address correct
                    TRS_DATA_OUT = data2_out;
                    TRS_DATA_TRIS = 0;
                    // release wait
                    TRS_WAIT = 1;
                    // wait for bus cycle to finish
                    while(GAL_INT == 0) ;
                    TRS_DATA_TRIS = 0xFF;
                    // reassert wait trigger
                    TRS_WAIT = 0;

                    TRS_DATA_OUT = data2_out = data2_out1;

                    // start next read
                    LATAbits.LATA5 = 0; // SD_CS = 0; // assert the SD CS
#if 1 // 64MHz
                    SPI1BAUD = 0x02; // fast clock
#else
                    SPI1BAUD = 0x01; // fast clock
#endif
                    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                    SPI1TCNTL = 1;
                    SPI1TXB = 255;

                    return;
                }
                TRS_DATA_OUT = extra_buffer[state_bytesdone2++];
                break;
            case 0x5: // 5 R CA 202
                // trs_read_seccnt
                TRS_DATA_OUT = state_seccnt;
                break;
            case 0x6: // 6 R C6 198
                // trs_read_uart_status
                TRS_DATA_OUT = 0;
                break;
            case 0x7: // 7 R CE 206
                // trs_read_sdh
                // sdh = essddhhh
                TRS_DATA_OUT = (state_secsize << 5) | (state_drive << 3) | state_head;
                break;
            case 0x8: // 8 R C1 193
                // trs_read_control
                TRS_DATA_OUT = state_control;
                break;
            case 0x9: // 9 R C9 201
                // trs_read_error
                TRS_DATA_OUT = state_error;
                break;
            case 0xA: // A R C5 197
                // trs_read_error2
                TRS_DATA_OUT = state_error2;
                break;
            case 0xB: // B R CD 205
                // trs_read_cylhi
                TRS_DATA_OUT = state_cyl >> 8;
                break;
            case 0xC: // C R C3 195
                // trs_read_size2
                TRS_DATA_OUT = state_size2;
                break;
            case 0xD: // D R CB 203
                // trs_read_secnum
                TRS_DATA_OUT = state_secnum;
                break;
            case 0xE: // E R C7 199
                // trs_read_uart
                TRS_DATA_OUT = 0;
                break;
            case 0xF: // F R CF 207 (74HC595)
                // int_ret_rd
                TRS_DATA_OUT = 0xFF;
                break;
            }
            TRS_DATA_TRIS = 0x00;
        }
        else
        {
            // write
            switch(TRS_ADDR & 0x0F)
            {
            case 0x0: // 0 W C0 192
                break;
            case 0x1: // 1 W C8 200
                // trs_write_data
                if(state_command == (TRS_HARD_WRITE >> 4))
                {
                    sector_buffer[state_bytesdone++] = TRS_DATA_IN;
                    if(state_bytesdone == state_secsize16)
                    {
                        SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                        SPI1TCNTL = 1;
                        SPI1TXB = new_status = (TRS_HARD_BUSY | TRS_HARD_CIP);
                        action_type = ACTION_HARD_WRITE;
                        action_flags |= ACTION_TRS;
                        state_bytesdone = 0;
                    }
                }
                break;
            case 0x2: // 2 W C4 196
                // trs_write_command2
                SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                SPI1TCNTL = 1;
                SPI1TXB = new_status = (TRS_HARD_BUSY | TRS_HARD_CIP);
                state_command2 = TRS_DATA_IN;
                state_bytesdone2 = 0;
                action_type = ACTION_EXTRA | (state_command2 & 0x1F);
                action_flags |= ACTION_TRS;
                break;
            case 0x3: // 3 W CC 204
                // trs_write_cyllo
                state_cyl = (state_cyl & 0xFF00) | TRS_DATA_IN;
                break;
            case 0x4: // 4 W C2 194
                // trs_write_data2
                extra_buffer[state_bytesdone2++] = TRS_DATA_IN;
                if(state_bytesdone2 == (UCHAR)state_size2)
                {
                    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                    SPI1TCNTL = 1;
                    SPI1TXB = new_status = (TRS_HARD_BUSY | TRS_HARD_CIP);
                    action_type = ACTION_EXTRA2 | (state_command2 & 0x1F);
                    action_flags |= ACTION_TRS;
                }
                break;
            case 0x5: // 5 W CA 202
                // trs_write_seccnt
                state_seccnt = TRS_DATA_IN;
                break;
            case 0x6: // 6 W C6 198
                // trs_write_uart_ctrl
                break;
            case 0x7: // 7 W CE 206
                // trs_write_sdh
                // sdh = essddhhh
                SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                SPI1TCNTL = 1;
                SPI1TXB = new_status = (TRS_HARD_READY | TRS_HARD_SEEKDONE);
                state_secsize = (TRS_DATA_IN >> 5) & 0x3;
                state_secsize16 = 256; // MAB!! fixme
                state_drive = (TRS_DATA_IN >> 3) & 0x3;
                state_head = TRS_DATA_IN & 0x7;
                break;
            case 0x8: // 8 W C1 193
                // trs_write_control
                state_control = TRS_DATA_IN;
                if(state_control & 0x08) // TRS_HARD_DEVICE_ENABLE = 0x8
                {
                    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                    SPI1TCNTL = 1;
                    if(state_present)
                    {
                        SPI1TXB = new_status = (TRS_HARD_READY | TRS_HARD_SEEKDONE);
                    }
                    else
                    {
                        SPI1TXB = new_status = (TRS_HARD_READY | TRS_HARD_SEEKDONE | TRS_HARD_ERR);
                        state_error = TRS_HARD_NFERR;
                    }
                }
                break;
            case 0x9: // 9 W C9 201
                // int_ret_rd
                break;
            case 0xA: // A W C5 197
                // trs_write_rom
                state_rom = TRS_DATA_IN;
                //SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                //SPI1TCNTL = 1;
                //SPI1TXB = new_status = state_rom;
                state_romdone = 0;
                break;
            case 0xB: // B W CD 205
                // trs_write_cylhi
                state_cyl = (state_cyl & 0x00FF) | (TRS_DATA_IN << 8);
                break;
            case 0xC: // C W C3 195
                // trs_write_size2
                state_size2 = TRS_DATA_IN;
                if(state_size2 == 0)
                    state_size2 = 0x100; // 0 --> 0x100
                break;
            case 0xD: // D W CB 203
                // trs_write_secnum
                state_secnum = TRS_DATA_IN;
                break;
            case 0xE: // E W C7 199
                // trs_write_uart
                break;
            case 0xF: // F W CF 207
                state_command = (TRS_DATA_IN >> 4) & 0xF; // lower nibble = command, save command here (save instructions later)
                state_bytesdone = 0;
                switch(state_command)
                {
                case 0x0: // F W CF 207
                    break;
                case 0x1: // F W CF 207 (restore)
                    // trs_write_cmd_restore
                    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                    SPI1TCNTL = 1;
                    SPI1TXB = new_status = (TRS_HARD_READY | TRS_HARD_SEEKDONE);
                    state_cyl = 0;
                    break;
                case 0x2: // F W CF 207 (read)
                    // trs_write_cmd_read
                    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                    SPI1TCNTL = 1;
                    SPI1TXB = new_status = (TRS_HARD_BUSY | TRS_HARD_CIP);
                    action_type = ACTION_HARD_READ;
                    action_flags |= ACTION_TRS;
                    break;
                case 0x3: // F W CF 207 (write)
                    // trs_write_cmd_write
                    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                    SPI1TCNTL = 1;
                    if(!(TRS_DATA_IN & TRS_HARD_MULTI))
                    {
                        SPI1TXB = new_status = (TRS_HARD_READY | TRS_HARD_SEEKDONE | TRS_HARD_DRQ);
                    }
                    else
                    {
                        SPI1TXB = new_status = (TRS_HARD_READY | TRS_HARD_SEEKDONE | TRS_HARD_ERR);
                        state_error = TRS_HARD_ABRTERR;
                    }
                    break;
                case 0x4: // F W CF 207 (verify)
                    // trs_write_cmd_verify
                    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                    SPI1TCNTL = 1;
                    SPI1TXB = new_status = (TRS_HARD_BUSY | TRS_HARD_CIP);
                    action_type = ACTION_HARD_SEEK;
                    action_flags = ACTION_TRS;
                    break;
                case 0x5: // F W CF 207 (format)
                    // trs_write_cmd_format
                    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                    SPI1TCNTL = 1;
                    SPI1TXB = new_status = (TRS_HARD_READY | TRS_HARD_SEEKDONE);
                    break;
                case 0x6: // F W CF 207 (init)
                    // trs_write_cmd_init
                    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                    SPI1TCNTL = 1;
                    SPI1TXB = new_status = (TRS_HARD_READY | TRS_HARD_SEEKDONE);
                    break;
                case 0x7: // F W CF 207 (seek)
                    // trs_write_cmd_seek
                    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                    SPI1TCNTL = 1;
                    SPI1TXB = new_status = (TRS_HARD_BUSY | TRS_HARD_CIP);
                    action_type = ACTION_HARD_SEEK;
                    action_flags = ACTION_TRS;
                    break;
                case 0x8: // F W CF 207 (no operation)
                    // trs_write_cmd_nop
                    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                    SPI1TCNTL = 1;
                    SPI1TXB = new_status = (TRS_HARD_READY | TRS_HARD_SEEKDONE);
                    break;
                case 0x9: // F W CF 207 (test - WD1002)
                    // trs_write_cmd_test
                    SPI1CON2 = _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
                    SPI1TCNTL = 1;
                    SPI1TXB = new_status = TRS_HARD_READY;
                    state_error = 0; // Clear error register
                    break;
                case 0xA: // F W CF 207 (no operation)
                case 0xB: // F W CF 207 (no operation)
                case 0xC: // F W CF 207 (no operation)
                case 0xD: // F W CF 207 (no operation)
                case 0xE: // F W CF 207 (no operation)
                case 0xF: // F W CF 207 (no operation)
                    // trs_write_cmd_nop
                    break;
                }
            }
        }
    }
    else
    {
        if(!(TRS_ADDR & 0x10)) // TRS read (0) or write (1) ?
        {
            TRS_DATA_OUT = state_status; // read => return status
            TRS_DATA_TRIS = 0x00;
        }
    }

    if(new_status)
    {
        state_status = new_status;
        while(!PIR3bits.SPI1RXIF) ;
        STAT_CS = 1;
        SPI1RXB;
        STAT_CS = 0;
    }

    // release wait
    TRS_WAIT = 1;
    // wait for bus cycle to finish
    while(GAL_INT == 0);
    TRS_DATA_TRIS = 0xFF;
    // reassert wait trigger
    TRS_WAIT = 0;
    }
}
