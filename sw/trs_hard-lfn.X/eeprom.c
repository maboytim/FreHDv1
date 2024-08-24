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
#include "mcc_generated_files/nvm/nvm.h"
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
	NVM_UnlockKeySet(UNLOCK_KEY);
	EEPROM_Write(EEPROM_START_ADDRESS+(uint24_t)address, value);
	NVM_UnlockKeyClear();
	while(NVM_IsBusy()) ;
}

