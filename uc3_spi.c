/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	File:	uc3_spi.c																	|
|	Description: Codedatei für AVR32 UC3x												|
|																						|
|																						|
|	Controller:																			|
|																						|
+---------------------------------------------------------------------------------------+
*/

#include <avr32/io.h>
#include "uc3_spi.h"

// Initialize SPI controller in master mode
// Chip select fixed to directly connected 4 devices (no 15 device decoder mode)
void SpiInitMaster(volatile avr32_spi_t *SpiInterface, uint8_t VariablePeripheralSelect, uint8_t FixedPeripheralSelectMask)
{
	//Reset SPI controller
	SpiInterface->cr = AVR32_SPI_CR_SWRST_MASK;
		
	//Setup master mode
	SpiInterface->mr = 0;
	SpiInterface->mr |= AVR32_SPI_MR_MSTR_MASK;											// Master mode
	SpiInterface->mr |= (VariablePeripheralSelect << AVR32_SPI_MR_PS_OFFSET);			// Write variable peripheral select
	SpiInterface->mr |= AVR32_SPI_MR_MODFDIS_MASK;										// Disable Mode Fault Detection
	SpiInterface->mr |= AVR32_SPI_MR_RXFIFOEN_MASK;										// Enable receive FIFO
	SpiInterface->mr |= (FixedPeripheralSelectMask << AVR32_SPI_MR_PCS_OFFSET);			// Write fixed select mask
}


//Initialize SPI controller in slave mode
void SpiInitSlave(volatile avr32_spi_t *SpiInterface)
{
	//Reset SPI controller
	SpiInterface->cr = AVR32_SPI_CR_SWRST;
	
	//Setup master mode
	SpiInterface->mr = 0;
	//SpiInterface->mr |= AVR32_SPI_MR_MSTR_MASK;											//Master mode
	//SpiInterface->mr |= 0;//(((1 << AVR32_SPI_MR_PCS_SIZE) - 1)<<AVR32_SPI_MR_PCS);		//No chip select on (low)
	
	//Enable SPI
	SpiInterface->cr = AVR32_SPI_CR_SPIEN_MASK;
}

//Enable SPI controller
void SpiEnable(volatile avr32_spi_t *SpiInterface)
{
	//Enable SPI
	SpiInterface->cr = AVR32_SPI_CR_SPIEN_MASK;
}

//Disable SPI controller
void SpiDisable(volatile avr32_spi_t *SpiInterface)
{
	//Disable SPI master
	SpiInterface->cr = AVR32_SPI_CR_SPIDIS_MASK;
}

//Calculate divider for chip baudrate
//From ATMEL ASF spi.c (11.10.2015)
int16_t getBaudDiv(const uint32_t baudrate, uint32_t pb_hz)
{
	uint32_t baudDiv = div_ceil(pb_hz, baudrate);

	if (baudDiv <= 0 || baudDiv > 255) {
		return -1;
	}

	return baudDiv;
}

//Set SPI controller settings for chosen slave chip
//For DataBits parameter use SPI_BITS_... constant
//For SpiMode parameter use SPI_MODE_... constant
void SpiSetChipSettings(volatile avr32_spi_t *SpiInterface, uint8_t Chip, uint8_t DataBits, uint8_t SpiMode, uint32_t Baudrate, uint32_t Clock)
{	
	uint8_t cpol = 0, ncpha = 1;
	uint32_t csrValue = 0;
	
	switch(SpiMode)
	{
		case 0:
			cpol = 0;
			ncpha = 1;
			break;
		case 1:
			cpol = 0;
			ncpha = 0;
			break;
		case 2:
			cpol = 1;
			ncpha = 1;
			break;
		case 3:
			cpol = 1;
			ncpha = 0;
			break;
	}
	
	//Calculate baudrate divider
	int baudDiv = getBaudDiv(Baudrate, Clock);
	if (baudDiv < 0)
		return;
	
	csrValue = (baudDiv<<AVR32_SPI_CSR0_SCBR) | (DataBits<<AVR32_SPI_CSR0_BITS) | (ncpha<<AVR32_SPI_CSR0_NCPHA) | (cpol<<AVR32_SPI_CSR0_CPOL);
	
	switch(Chip)
	{
		case 0:
			SpiInterface->csr0 = csrValue;
			break;
		case 1:
			SpiInterface->csr1 = csrValue;
			break;
		case 2:
			SpiInterface->csr2 = csrValue;
			break;
		case 3:
			SpiInterface->csr3 = csrValue;
			break;
	}
}

//Get the status register
//defines SPI_STATUS_... give masks for the status bits
uint16_t SpiGetStatus(volatile avr32_spi_t *SpiInterface)
{
	return SpiInterface->sr;
}


//Check if SPI controller send register is empty
uint8_t SpiSendRegisterEmpty(volatile avr32_spi_t *SpiInterface)
{
	return ((SpiInterface->sr & AVR32_SPI_SR_TDRE_MASK) != 0);
}


//Enable SPI interrupt
//For bit number please refer to the data sheet
//defines SPI_INT_... represent the bit numbers
void SpiEnableInterrupt(volatile avr32_spi_t *SpiInterface, uint8_t bit)
{
	SpiInterface->ier |= (1<<bit);
}


//Disable SPI interrupt
//For bit number please refer to the data sheet
//defines SPI_INT_... represent the bit numbers
void SpiDisableInterrupt(volatile avr32_spi_t *SpiInterface, uint8_t bit)
{
	SpiInterface->idr |= (1<<bit);
}


//Get interrupt mask register
uint32_t SpiGetInterruptMask(volatile avr32_spi_t *SpiInterface)
{
	return SpiInterface->imr;
}


//Get the received value
uint16_t SpiGetRecByte(volatile avr32_spi_t *SpiInterface)
{
	return SpiInterface->rdr;
}


//Wait for transmission finished
void SpiWaitForTxFinished(volatile avr32_spi_t *SpiInterface)
{
	while((SpiInterface->sr & AVR32_SPI_SR_TXEMPTY_MASK) == 0);
}


//Transmit a single byte
void SpiSendSingleByte(volatile avr32_spi_t *SpiInterface, uint16_t TxWord)
{
	SpiInterface->tdr = TxWord;
}


//Transmit multiple bytes
void SpiSendMultipleBytes(volatile avr32_spi_t *SpiInterface, uint16_t *TxWords, uint8_t num)
{
	uint8_t i;
	
	for(i=0; i<num; i++)
	{
		while(SpiSendRegisterEmpty(SpiInterface) == 0);
		SpiSendSingleByte(SpiInterface, TxWords[i]);
	}
}


//Read one byte from slave
uint16_t SpiReadSingleByte(volatile avr32_spi_t *SpiInterface, uint16_t TxWord)
{
	SpiSendSingleByte(SpiInterface, TxWord);
	return SpiInterface->rdr;
}


//Read multiple bytes
void SpiReadMultipleBytes(volatile avr32_spi_t *SpiInterface, uint16_t *RxWords, uint16_t *TxWords, uint8_t num)
{
	uint8_t i;
	
	for(i=0; i<num; i++)
	{
		SpiWaitForTxFinished(SpiInterface);
		RxWords[i] = SpiReadSingleByte(SpiInterface, TxWords[i]);
	}
}
