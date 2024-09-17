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

#include <stdint.h>
#include "HardwareProfile.h"


#define SERIAL_BUF_OUT_SIZE		128
#define SERIAL_BUF_IN_SIZE		128

#pragma udata uart_buffer
static uint8_t serial_out[SERIAL_BUF_OUT_SIZE];
static uint8_t serial_in[SERIAL_BUF_IN_SIZE];
#pragma udata
static uint8_t tx_count, tx_in, tx_out;
static uint8_t rx_count, rx_in, rx_out;
static uint8_t discard;


void usart_init(void)
{
	TX_TRIS = 0;
	RX_TRIS = 1;
	RTS_TRIS = 0;
	CTS_TRIS = 1;
    //UTXI - low priority
    IPR4bits.U1TXIP = 0;
    //URXI - low priority
    IPR4bits.U1RXIP = 0;
    //
    U1RXB = 0x0;
    //RXCHK disabled;
    U1RXCHK = 0x0;
    //TXB disabled;
    U1TXB = 0x0;
    //TXCHK disabled;
    U1TXCHK = 0x0;
    //P1L 0x0;
    U1P1L = 0x0;
    //P1H 0x0;
    U1P1H = 0x0;
    //P2L 0x0;
    U1P2L = 0x0;
    //P2H 0x0;
    U1P2H = 0x0;
    //P3L 0x0;
    U1P3L = 0x0;
    //P3H 0x0;
    U1P3H = 0x0;
    //MODE Asynchronous 8-bit mode; RXEN enabled; TXEN enabled; ABDEN disabled; BRGS high speed; 
    U1CON0 = 0xB0;
    //SENDB disabled; BRKOVR disabled; RXBIMD Set RXBKIF on rising RX input; WUE disabled; ON enabled; 
    U1CON1 = 0x80;
    //FLO off; TXPOL not inverted; C0EN Add all TX and RX characters; STP Transmit 1Stop bit, receiver verifies first Stop bit; RXPOL not inverted; RUNOVF RX input shifter stops all activity; 
    U1CON2 = 0x8;
#if 1 // 64MHz
    //BRGL 138;
    U1BRGL = 0x8A;
#else // 40MHz
    //BRGL 86;
    U1BRGL = 0x56;
#endif
    //BRGH 0;
    U1BRGH = 0x0;
    //TXBE empty; STPMD in middle of first Stop bit; TXWRE No error; 
    U1FIFO = 0x2E;
    //ABDIE disabled; ABDIF Auto-baud not enabled or not complete; WUIF WUE not enabled by software; 
    U1UIR = 0x0;
    //TXCIF equal; RXFOIF not overflowed; RXBKIF No Break detected; FERIF no error; CERIF No Checksum error; ABDOVF Not overflowed; PERIF no parity error; TXMTIF empty; 
    U1ERRIR = 0x80;
    //TXCIE disabled; RXFOIE disabled; RXBKIE disabled; FERIE disabled; CERIE disabled; ABDOVE disabled; PERIE disabled; TXMTIE disabled; 
    U1ERRIE = 0x0;
	PIE4bits.U1RXIE = 1;
	
	tx_count = 0;
	tx_in = 0;
	tx_out = 0;
	rx_count = 0;
	rx_in = 0;
	rx_out = 0;
}	


void serial_tx_int(void)
{
	if (tx_count == 0) {
		// nothing to send. disable interrupt
		PIE4bits.U1TXIE = 0;
		return;
	}
	U1TXB = serial_out[tx_out];
	if (++tx_out == SERIAL_BUF_OUT_SIZE) {
		tx_out = 0;
	}
	tx_count--;
}	


uint8_t usart_ok_to_send(void)
{
	return (tx_count < SERIAL_BUF_OUT_SIZE);
}	

void usart_send(uint8_t data)
{
	while (tx_count >= SERIAL_BUF_OUT_SIZE);
	INTCON0bits.GIEL = 0;
	serial_out[tx_in] = data;
	if (++tx_in == SERIAL_BUF_OUT_SIZE) {
		tx_in = 0;
	}
	tx_count++;
	PIE4bits.U1TXIE = 1;
	INTCON0bits.GIEL = 1;	
}

void usart_flush(void)
{
	while (tx_count != 0);
}	

void serial_rx_int(void)
{
	if (U1ERRIRbits.RXFOIF) {
		// reset receive bit after overrun error
		//RCSTAbits.CREN = 0;
		//RCSTAbits.CREN = 1;
	}
	if (U1ERRIRbits.FERIF == 0) {
		if (rx_count < SERIAL_BUF_IN_SIZE) {
			serial_in[rx_in] = U1RXB;
			if (++rx_in == SERIAL_BUF_IN_SIZE) {
				rx_in = 0;
			}
			rx_count++;
			
			return;
		}	
	}	
	// discard byte
	discard = U1RXB;
	
	return;
}


uint8_t usart_get(uint8_t *data)
{
	if (rx_count == 0) return 0;
	INTCON0bits.GIEL = 0;
	*data = serial_in[rx_out];
	if (++rx_out == SERIAL_BUF_IN_SIZE) {
		rx_out = 0;
	}
	rx_count--;
	INTCON0bits.GIEL = 1;
	
	return 1;
}		


void usart_puts(const __rom char *str)
{
	while (*str) {
		usart_send(*(str++));
	}		
}

void usart_puts2(const __rom char *str)
{
	usart_puts(str);
	usart_send('\r');
	usart_send('\n');	
}


void usart_puts_r(const char *str)
{
	while (*str) {
		usart_send(*(str++));
	}		
}

void usart_puts2_r(const char *str)
{
	usart_puts_r(str);
	usart_send('\r');
	usart_send('\n');	
}
	
void usart_put_hex(char c)
{
	char x;
	
	x = (c & 0xF0) >> 4;
	if (x > 9) 
		x += 'A' - 10;
	else
		x += '0';
	usart_send(x);
	x = c & 0x0F;
	if (x > 9)
		x += 'A' - 10;
	else
		x += '0';
	usart_send(x);
}	


void usart_put_short(short x)
{
	usart_put_hex((x >> 8) & 0xff);
	usart_put_hex(x & 0xff);	
}	

void usart_put_long(long x)
{
	usart_put_hex((x >> 24) & 0xff);
	usart_put_hex((x >> 16) & 0xff);
	usart_put_hex((x >> 8) & 0xff);
	usart_put_hex(x & 0xff);
}

#if 0 // if using printf to uart
void putch(char txData)
{
    if(txData == '\n')
    {
        usart_send('\r');
    }
    usart_send(txData);   
}
#endif
