#include "ff.h"			/* Obtains integer types */
#include "../integer.h"			/* Obtains integer types */

extern __near BYTE crc7;
extern __near USHORT crc16;

/*
* CRC-7  (x^7+x^3+x^0)
*/
void mmc_crc7(BYTE x)
{
	BYTE ibit;
	for (ibit = 8; ibit; ibit--)
	{
		crc7 <<= 1;
		if ((x ^ crc7) & 0x80)
			crc7 ^= 0x09;
		x <<= 1;
	}
	crc7 &= 0x7F;
}

/*
* CRC-16  (x^16+x^12+x^5+x^0)
*/
void mmc_crc16(BYTE x)
{
	BYTE ibit;
	for (ibit = 8; ibit; ibit--)
	{
		if ((x ^ (crc16 >> 8)) & 0x80)
			crc16 = (crc16 << 1) ^ 0x1021;
		else
			crc16 <<= 1;
		x <<= 1;
	}
}
