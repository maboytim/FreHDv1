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
#include "eeprom.h"

/*
 * read one byte from EEPROM
 */
uint8_t ee_read8(uint16_t address)
{
	return (*(const uint8_t *)(EEPROM_START_ADDRESS+(uint24_t)address));
}


/*
 * write one byte into EEPROM
 */
void ee_write8(uint16_t address, uint8_t value)
{
    //Load NVMADR with the target address of the byte
    NVMADRU = (uint8_t) (EEPROM_START_ADDRESS >> 16);
    NVMADRH = (uint8_t) (address >> 8);
    NVMADRL = (uint8_t) address;

    //Load NVMDAT with the desired value
    NVMDATL = value;

    //Set the byte write command
    NVMCON1bits.NVMCMD = 0x03;

    //Disable global interrupt
    INTCON0bits.GIE = 0;
    
    //Perform the unlock sequence 
    asm("asmopt push"); //save the current selection of optimizations
    asm("asmopt off"); //turn off assembler optimizations
    asm("banksel(_NVMLOCK)"); //select the bank of the NVMLOCK register
    NVMLOCK = 0x55; //assign 'unlockKeyLow' to NVMLOCK.
    asm("MOVLW 0xAA"); //load 'unlockKeyHigh' into the W register
    asm("MOVWF (_NVMLOCK&0xFF),b"); //move the W register to NVMLOCK

    //Start byte write
    asm("bsf (_NVMCON0bits&0xFF)," ___mkstr(_NVMCON0_GO_POSN) ",b"); //Set GO bit   
    asm("asmopt pop"); //restore assembler optimization settings

    //Restore global interrupt enable bit value
    INTCON0bits.GIE = 1;

    //Clear the NVM Command
    NVMCON1bits.NVMCMD = 0x00;

    while(NVMCON0bits.GO) ;
}

