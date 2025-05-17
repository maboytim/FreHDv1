/**
  * SPI1 Generated Driver File
  *
  * @file spi1.c
  *
  * @ingroup spi1
  *
  * @brief This file contains the driver code for the SPI1 module.
  *
  * @version SPI1 Driver Version v3.1.0.
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include <xc.h>
#include "../spi_interface.h"
#include "../spi1.h"
#include "../spi_polling_types.h"

/**
 * @ingroup spi1
 * @struct SPI_INTERFACE SPI1
 * @brief Defines an object for SPI_INTERFACE functions.
 */ 
 
const struct SPI_INTERFACE SPI1_Host = {
    .Initialize = SPI1_Initialize,
    .Deinitialize = SPI1_Deinitialize,
    .Open = SPI1_Open,
    .Close = SPI1_Close,
    .BufferExchange = SPI1_BufferExchange,
    .BufferWrite = SPI1_BufferWrite,
    .BufferRead = SPI1_BufferRead,	
    .ByteExchange = SPI1_ByteExchange,
    .ByteWrite = SPI1_ByteWrite,
    .ByteRead = SPI1_ByteRead,
    .IsTxReady = SPI1_IsTxReady,
    .IsRxReady = SPI1_IsRxReady,
    .RxCompleteCallbackRegister = NULL,
    .TxCompleteCallbackRegister = NULL
};

//con0 == SPI1CON0, con1 == SPI1CON1, con2 == SPI1CON2, baud == SPI1BAUD, clksel == SPI1CLKSEL
static const spi_configuration_t spi1_configuration[] = {   
    { 0x2, 0x40, 0x0, 0x2, 0x0 },
    { 0x2, 0x40, 0x0, 0x0, 0x0 }
};

void SPI1_Initialize(void)
{
    //Disable the interrupts
    PIE3bits.SPI1IE 		= 0U;
    PIE3bits.SPI1TXIE 	= 0U;
    PIE3bits.SPI1RXIE 	= 0U;
	
	//Clear all registers to reset value
    SPI1CON0 = 0x00U;
    SPI1CON1 = 0x00U;
    SPI1CON2 = 0x00U;
    SPI1BAUD = 0x00U;
    SPI1CLK = 0x00U;
}

void SPI1_Deinitialize(void)
{
    SPI1CON0bits.EN = 0U;
}

bool SPI1_Open(uint8_t spiConfigIndex)
{
	bool returnValue = false; 
	if(false == SPI1CON0bits.EN)
	{
        SPI1CON0 = spi1_configuration[spiConfigIndex].con0;
        SPI1CON1 = spi1_configuration[spiConfigIndex].con1;
        SPI1CON2 = spi1_configuration[spiConfigIndex].con2 & ~(_SPI1CON2_SPI1RXR_MASK) & ~(_SPI1CON2_SPI1TXR_MASK);
        SPI1BAUD = spi1_configuration[spiConfigIndex].baud;        
        SPI1CLK = spi1_configuration[spiConfigIndex].clksel;        
		TRISCbits.TRISC3 = 0;
        SPI1CON0bits.EN = 1U;
		
		returnValue = true;
	}
	else
	{
		returnValue = false;
	}
	return returnValue;
}

void SPI1_Close(void)
{
	SPI1CON0bits.EN = 0U;
}

void SPI1_BufferExchange(void *bufferData, size_t bufferSize)
{
	uint8_t* bufferInput = bufferData;

	//Load the transfer count register
	SPI1TCNTH = (uint8_t)(bufferSize>>8);
	SPI1TCNTL = (uint8_t)(bufferSize);	
	
	//Set both SPI as full duplex mode for buffer exchange operation
    SPI1CON2 = ((SPI1CON2) | (_SPI1CON2_SPI1RXR_MASK) | (_SPI1CON2_SPI1TXR_MASK));
	
	while(0U == SPI1INTFbits.TCZIF)
	{
		//Write input data to SPI transmit buffer register
		SPI1TXB = *bufferInput;
		while(!PIR3bits.SPI1RXIF)
		{
			//Wait on receive interrupt flag to be set
        };
		
		//Store received data  from receive buffer register 
		*bufferInput = SPI1RXB;		
		bufferInput++;
	}
	//Clear the TCZIF interrupt flag
	SPI1INTFbits.TCZIF = 0U;
}

void SPI1_BufferWrite(void *bufferData, size_t bufferSize)
{
	uint8_t* bufferInput = bufferData;

	//Load the transfer count register
	SPI1TCNTH = (uint8_t)(bufferSize>>8);
	SPI1TCNTL = (uint8_t)(bufferSize);

	//Set both SPI as transmit only mode for buffer write operation
    SPI1CON2 = (SPI1CON2 & ~(_SPI1CON2_SPI1RXR_MASK)) | (_SPI1CON2_SPI1TXR_MASK);
   
	while(0U == SPI1INTFbits.TCZIF)
	{
		//Write input data to SPI transmit buffer register
		SPI1TXB = *bufferInput;
		while(!PIR3bits.SPI1TXIF)
		{
			//Wait on transmit interrupt flag to be set
        };
		bufferInput++;
	}	
	//Clear the TCZIF interrupt flag
	SPI1INTFbits.TCZIF = 0U;
}

void SPI1_BufferRead(void *bufferData, size_t bufferSize)
{
	uint8_t* bufferInput = bufferData;
	
	//Load the transfer count register
	SPI1TCNTH = (uint8_t)(bufferSize>>8);
	SPI1TCNTL = (uint8_t)(bufferSize);

	//Set both SPI as receive only mode for buffer read operation
    SPI1CON2 = (SPI1CON2 | (_SPI1CON2_SPI1RXR_MASK)) & ~(_SPI1CON2_SPI1TXR_MASK);
	while(0U == SPI1INTFbits.TCZIF)
	{
		//Write dummy data to SPI transmit buffer register
		SPI1TXB = 0x00U;
		while(!PIR3bits.SPI1RXIF)
		{
            //Wait on receive interrupt flag to be set
        };
		//Store received data  from receive buffer register 
		*bufferInput = SPI1RXB;
		bufferInput++;
	}
	//Clear the TCZIF interrupt flag
	SPI1INTFbits.TCZIF = 0U;
}

uint8_t SPI1_ByteExchange(uint8_t byteData)
{
    SPI1CON2 = SPI1CON2 | _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
    uint8_t returnValue = 0U;
	SPI1TCNTL = 1;
	//Write input data to SPI transmit buffer register
	SPI1TXB = byteData;
	while(!PIR3bits.SPI1RXIF)
	{
		//Wait on receive interrupt flag to be set
    };	
	
	//Store received data  from receive buffer register 
	returnValue = SPI1RXB;
	
	return returnValue;
}

void SPI1_ByteWrite(uint8_t byteData)
{	
	SPI1CON2 = SPI1CON2 | _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;
	SPI1TCNTL = 1;
	//Write input data to SPI transmit buffer register
	SPI1TXB = byteData;
}

uint8_t SPI1_ByteRead(void)
{
	uint8_t returnValue = 0x00U;
    SPI1CON2 = SPI1CON2 | _SPI1CON2_SPI1RXR_MASK | _SPI1CON2_SPI1TXR_MASK;

	//Store received data  from receive buffer register 
	returnValue = SPI1RXB;	
	return returnValue;
}


bool SPI1_IsTxReady(void)
{
	bool returnValue = false;
	if(true == SPI1CON0bits.EN)
	{
		returnValue = PIR3bits.SPI1TXIF;
	}
	else 
	{
		returnValue = false;
	}
	return returnValue;
}

bool SPI1_IsRxReady(void)
{
	bool returnValue = false;
	
	if(true == SPI1CON0bits.EN)
    {
		returnValue = PIR3bits.SPI1RXIF;
    }
    else 
    {
        returnValue = false;
    }
    return returnValue;
}
