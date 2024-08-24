/*
 * Copyright (C) 2013 Frederic Vecoven
 *
 * This file is part of trs_hard
 *
 * trs_hard is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * trs_hard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "HardwareProfile.h"
#include "mcc_generated_files/system/system.h"
#include <string.h>
#include "integer.h"
#include "FatFS/ff.h"
#include "FatFS/diskio.h"
#include "trs_hard.h"
#include "trs_extra.h"
//#include "trs_uart.h"
#include "sw_i2c.h"
#include "ds1307.h"
#include "action.h"
#include "serial.h"
#include "led.h"
#include "bootloader.h"
#include "crc.h"

const USHORT crc16_val __at(FLASH_CRC_ADDR) = THECRC;
const USHORT exit_boot __at(EEPROM_START_ADDRESS + EEPROM_CRC_ADDR) = THECRC;

/* Global variables */
#pragma udata fs_buffer
FATFS fs;
Drive state_d[TRS_HARD_MAXDRIVES];
FIL state_file2;
#if EXTRA_IM_SUPPORT
#if _USE_FASTSEEK
DWORD im_stbl[FAST_SEEK_LEN];
#endif
FIL im_file;
BYTE im_buf[0x80];
#endif
BYTE sector_buffer[MAX_SECTOR_SIZE];
#if EXTRA_IM_SUPPORT
image_t im[8];
#endif
#pragma udata extra_buffer
BYTE extra_buffer[EXTRA_SIZE];
#pragma udata
DIR state_dir;
FILINFO state_fno;
UCHAR state_dir_open;
UCHAR state_file2_open;
UCHAR fs_mounted;
UCHAR cur_unit;
UCHAR led_count;
led_t rled;
led_t gled;

__near UCHAR expected_addr;

__near BYTE action_flags;
__near BYTE action_type;
__near BYTE action_status;
__near UCHAR state_busy;
__near UCHAR state_status;
__near UCHAR state_present;
__near UCHAR state_control;
__near UCHAR state_error;
__near UCHAR state_seccnt;
__near UCHAR state_secnum;
__near USHORT state_cyl;
__near UCHAR state_drive;
__near UCHAR state_head;
__near UCHAR state_wp;
__near UCHAR state_command;
__near USHORT state_bytesdone;
__near UCHAR state_secsize;
__near USHORT state_secsize16;
__near UCHAR state_command2;
__near UCHAR state_error2;
__near USHORT state_size2;
__near UCHAR state_bytesdone2;
__near UCHAR state_romdone;
__near UCHAR state_rom;
__near UCHAR val_1F;
__near UCHAR data2_out, data2_out1;
__near UCHAR data2_stream;
__near UCHAR crc7;
__near USHORT crc16;
__near UCHAR foo;
__near UCHAR gled_val;

/* local prototypes */
void handle_interrupt_low(void);


/*
 * Interrupt handlers
 */

asm(" global handle_int2");
asm("psect intcode,class=CODE,space=0,reloc=2,abs,ovrld");
asm(" org  0x1000");
asm(" goto start");
asm(" org  0x1008");
asm(" goto handle_int2");
asm(" org  0x1018");
asm(" goto _handle_interrupt_low");

void __interrupt(low_priority) handle_interrupt_low(void)
{
    if(PIE3bits.TMR2IE && PIR3bits.TMR2IF) {
		PIR3bits.TMR2IF = 0;
		disk_timerproc();
		action_flags |= ACTION_TICK;
	}
    if(RTC_INT_IE && RTC_INT_IF) {
		RTC_INT_IF = 0;
		ds1307_int();
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


void pic_init(void)
{
	UCHAR i;

	IVTLOCK = 0x55;
	IVTLOCK = 0xAA;
	IVTLOCKbits.IVTLOCKED = 0x00; // unlock IVT

	IVTBASEU = 0;
	IVTBASEH = 16;
	IVTBASEL = 8;

	IVTLOCK = 0x55;
	IVTLOCK = 0xAA;
	IVTLOCKbits.IVTLOCKED = 0x01; // lock IVT

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
	GAL_INT_IE = 1;
	GAL_INT_IP = 1;
	GAL_INT_EDGE = 0;	// interrupt on falling edge

	/* DS1307 interrupt */
	RTC_INT_TRIS = INPUT_PIN;
	RTC_INT_IE = 1;
	RTC_INT_IP = 0;
	RTC_INT_EDGE = 0;

	/* Debug LED */
	GLED_TRIS = OUTPUT_PIN;
	RLED_TRIS = OUTPUT_PIN;
	GLED = 0;
	RLED = 0;

	/* Timer 2 : ~200Hz */
//	OpenTimer2(TIMER_INT_ON & T2_PS_1_16 & T2_POST_1_16);
//	PR2 = 195;
	IPR3bits.TMR2IP = 0;

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

	/* globals init */
	val_1F = 0x1F;
	expected_addr = 0xff; // won't match any input
	action_flags = 0;
	fs_mounted = FS_NOT_MOUNTED;
	state_file2_open = 0;
	state_dir_open = 0;
	for (i = 0; i < TRS_HARD_MAXDRIVES; i++) {
		state_d[i].filename[0] = '\0';
	}
}


static void handle_card_insertion(void)
{
	FRESULT rc;
	BYTE retry;

	gled.val |= LED_FLASH;
	fs_mounted = FS_MOUNTED_ERROR;
#if UART_DEBUG
	usart_puts2("Card inserted!");
#endif
	retry = 3;
	while ((rc = disk_initialize(0)) != RES_OK && retry-- > 0);
	if (rc == RES_OK) {
#if UART_DEBUG
		usart_puts2("Card initialized!");
#endif
		rc = f_mount(&fs, "", 1);
		if (rc == FR_OK) {
			fs_mounted = FS_MOUNTED_OK;
#if UART_DEBUG
			usart_puts2("FAT mounted");
#endif
			rc = open_drives();
#if UART_DEBUG
			if (rc == FR_OK) {
				usart_puts2("Drives mounted");
			}
#endif
		}
	}
	if (fs_mounted == FS_MOUNTED_ERROR) {
#if UART_DEBUG
		usart_puts2("Card init failed.");
#endif
		rled.val = LED_FAST_BLINK;
	}

	// capture result
	state_error2 = rc;
}


static void handle_card_removal(void)
{
#if UART_DEBUG
	usart_puts2("Card removed!");
#endif
	close_drives();
	f_unmount("");
	fs_mounted = 0;
	rled.val = LED_OFF;
}


void main(void)
{
	SYSTEM_Initialize();
	pic_init();
	led_init();
	i2c_init();
	ds1307_init();
	trs_hard_init();
	usart_init();
	TMR2_Start();

	/* Set READY status */
	state_busy = 0;
	update_status(TRS_HARD_READY);

	/* Enable interrupts */
	INTCON0bits.GIE = 1;
	INTCON0bits.GIEL = 1;

	// assert wait trigger
	TRS_WAIT = 0;

	while (1) {

		/* DS1307 : reload time from chip */
		if (action_flags & ACTION_DS1307_RELOAD) {
			ds1307_read_time();
			action_flags &= ~ACTION_DS1307_RELOAD;
		}

		/* tick 200Hz */
		if (action_flags & ACTION_TICK) {
			action_flags &= ~ACTION_TICK;

			/* handle LEDs */
			led_update(&rled);
			gled.val |= gled_val;
			gled_val = 0;
			led_update(&gled);

			/* sync hard drive files */
			trs_sync();

			/* card insertion */
			if (card_present() && fs_mounted == FS_NOT_MOUNTED) {
				handle_card_insertion();
			}

			/* card removal */
			if (!card_present() && fs_mounted != FS_NOT_MOUNTED) {
				handle_card_removal();
			}
		}

		/* hard drive and extra functions actions */
		if (action_flags & ACTION_TRS) {
			action_flags &= ~ACTION_TRS;
			trs_action();
		}

#if 0
		/* UART */
		trs_uart();
#endif
	}
}
