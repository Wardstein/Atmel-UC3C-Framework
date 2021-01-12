/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	File:	uc3_twi.c																	|
|	Description: Codedatei für AVR32 UC3C												|
|																						|
|																						|
|	Controller: AT32UC3Cxxxx															|
|																						|
+---------------------------------------------------------------------------------------+
*/

#include <avr32/io.h>
#include "uc3_twi.h"

#include "../config.h"
#include "uc3_io.h"

#include "uc3_usart.h"


//Set the frequency of the TWI
//Source: Atmel Software Framework - TWI driver - twi.c (25.12.2014)
void TwiSetFrequency(volatile avr32_twim_t *twi, uint32_t speed, uint32_t pba_hz)
{
	uint32_t f_prescaled;
	uint8_t cwgr_exp = 0;
	f_prescaled = (pba_hz / speed / 2);
	// f_prescaled must fit in 8 bits, cwgr_exp must fit in 3 bits
	while ((f_prescaled > 0xFF) && (cwgr_exp <= 0x7)) {
		// increase clock divider
		cwgr_exp++;
		// divide f_prescaled value
		f_prescaled /= 2;
	}
	if (cwgr_exp > 0x7) {
		return;
	}
	// set clock waveform generator register
	twi->cwgr = ((f_prescaled/2) << AVR32_TWIM_CWGR_LOW_OFFSET)
				| ((f_prescaled - f_prescaled/2) << AVR32_TWIM_CWGR_HIGH_OFFSET)
				| (cwgr_exp << AVR32_TWIM_CWGR_EXP_OFFSET)
				| (0     << AVR32_TWIM_CWGR_DATA_OFFSET)
				| (f_prescaled << AVR32_TWIM_CWGR_STASTO_OFFSET);
}


//Initialize twi controller in master mode
void TwiInitMaster(volatile avr32_twim_t *twi, uint32_t Speed, uint32_t Clock)
{
	//Enable clock of TWIM 0 & 1
	//AVR32_PM.pbamask |= (1<<13) | (1<<12);
	
	////Set to master mode
	//twi->cr = AVR32_TWIM_CR_MEN_MASK;
//
	////Reset TWI controller
	//twi->cr = AVR32_TWIM_CR_SWRST_MASK;	
	//
	////Reset status
	//twi->scr = 0xFFFFFFFF;
	//twi->sr;
	//
	////Switch off
	//twi->cr = AVR32_TWIM_CR_MDIS_MASK;
		
	//while(((AVR32_TWIM1.sr) & TWI_STATUS_MENB) == 0);
	
	//Set twi frequency
	TwiSetFrequency(twi, Speed, Clock);
}


//Start the twi transmission
void TwiSendStart(volatile avr32_twim_t *twi)
{
	twi->cmdr |= AVR32_TWIM_CMDR_START_MASK;
}


//A stop condition will be transmitted right after the actual transmitted byte
void TwiSendStop(volatile avr32_twim_t *twi)
{
	twi->cmdr |= AVR32_TWIM_CR_STOP_MASK;
}


//Set address of the slave
void TwiSetSlaveAddress(volatile avr32_twim_t *twi, uint8_t SlaveAddress)
{
	twi->cmdr &= ~AVR32_TWIM_CMDR_SADR_MASK;
	twi->cmdr |= (SlaveAddress<<AVR32_TWIM_CMDR_SADR);
}


//Set write (0) / read (1) mode
void TwiSetDirection(volatile avr32_twim_t *twi, uint8_t mode)
{
	if(mode > 1)
		mode = 1;
		
	twi->cmdr &= ~AVR32_TWIM_CMDR_READ_MASK;
	twi->cmdr |= (mode<<AVR32_TWIM_CMDR_READ);
}


//Get the status register
//defines TWI_STATUS_... give masks for the status bits
uint32_t TwiGetStatus(volatile avr32_twim_t *twi)
{
	return twi->sr;
}


//Enable twi interrupt
//For bit number please refer to the data sheet
//defines TWI_INT_... represent the bit numbers
void TwiEnableInterrupt(volatile avr32_twim_t *twi, uint8_t bit)
{
	twi->ier |= (1<<bit);
}


//Disable twi interrupt
//For bit number please refer to the data sheet
//defines TWI_INT_... represent the bit numbers
void TwiDisableInterrupt(volatile avr32_twim_t *twi, uint8_t bit)
{
	twi->idr |= (1<<bit);
}


//Get interrupt mask register
uint32_t TwiGetInterruptMask(volatile avr32_twim_t *twi)
{
	return twi->imr;
}


//Get the received byte
uint8_t TwiGetRecByte(volatile avr32_twim_t *twi)
{
	return (twi->rhr & 0xFF);
}


//Write byte into transmit register
void TwiTransmitByte(volatile avr32_twim_t *twi, uint8_t byte)
{
	twi->thr = byte;
}


//Clear all status bits
void TwiClearStatus(volatile avr32_twim_t *twi)
{
	twi->scr = 0xFFFFFFFF;
}

/*
//Transmit a single byte
void TwiSendSingleByte(volatile avr32_twim_t *twi, uint8_t SlaveAddress, uint8_t byte)
{
	//Store slave address
	TwiSetSlaveAddress(twi, SlaveAddress);
	
	//Write mode
	TwiSetDirection(0);
	
	//Store byte to send
	TwiTransmitByte(twi, byte);
	
	//Wait for byte transmitted
	while((TwiGetStatus() & TWI_STATUS_TXRDY) == 0);
	
	//Wait for stop send
	while((TwiGetStatus() & TWI_STATUS_TXCOMP) == 0);
}
*/

//Transmit multiple bytes
uint32_t TwiSendBytes(volatile avr32_twim_t *twi, uint8_t SlaveAddress, uint8_t nInternalAdress, uint8_t *InternalAddress, uint8_t num, uint8_t *bytes)
{
	uint8_t i;

	//Reset TWI master to clear THR and SR register
	twi->cr = AVR32_TWIM_CR_MEN_MASK;
	twi->cr = AVR32_TWIM_CR_SWRST_MASK;
	twi->scr = 0xFFFFFFFF;
	twi->cr = AVR32_TWIM_CR_MDIS_MASK;

	//Set TX command
	twi->cmdr = (SlaveAddress << AVR32_TWIM_CMDR_SADR_OFFSET) |
				((nInternalAdress + num) << AVR32_TWIM_CMDR_NBYTES_OFFSET) | 
				(AVR32_TWIM_CMDR_VALID_MASK) |
				(AVR32_TWIM_CMDR_START_MASK) |
				(AVR32_TWIM_CMDR_STOP_MASK) | 
				(0 << AVR32_TWIM_CMDR_READ);

	//Enable TWI master
	twi->cr = AVR32_TWIM_CR_MEN_MASK;

	//Wait for data ready
	while((twi->sr & AVR32_TWIM_SR_TXRDY_MASK) == 0)
	{
		//Check address fail flag and return
		if((twi->sr & AVR32_TWIM_SR_ANAK_MASK))
			return TWI_STATUS_ANAK;
	}
	
	//Transmit internal address
	for(i=0; i<nInternalAdress; i++)
	{
		//Wait for byte transmitted
		while((twi->sr & AVR32_TWIM_SR_TXRDY_MASK) == 0)
		{
			//Check address fail flag and return
			if((twi->sr & AVR32_TWIM_SR_ANAK_MASK))
				return TWI_STATUS_ANAK;
			//Check data fail flag and return
			if((twi->sr & AVR32_TWIM_SR_DNAK_MASK))
				return TWI_STATUS_DNAK;
		}

		//Send next byte
		TwiTransmitByte(twi, *InternalAddress++);
	}
	
	//Transmit data bytes
	for(i=0; i<num; i++)
	{
		//Wait for byte transmitted
		while((twi->sr & AVR32_TWIM_SR_TXRDY_MASK) == 0)
		{
			//Check address fail flag and return
			if((twi->sr & AVR32_TWIM_SR_ANAK_MASK))
				return TWI_STATUS_ANAK;
			//Check data fail flag and return
			if((twi->sr & AVR32_TWIM_SR_DNAK_MASK))
				return TWI_STATUS_DNAK;
		}		

		//Send next byte
		TwiTransmitByte(twi, *bytes++);
	}
		
	//Wait for stop
	while((TwiGetStatus(twi) & TWI_STATUS_CCOMP) == 0)
	{
		//Check address fail flag and return
		if((twi->sr & AVR32_TWIM_SR_ANAK_MASK))
			return TWI_STATUS_ANAK;
		//Check data fail flag and return
		if((twi->sr & AVR32_TWIM_SR_DNAK_MASK))
			return TWI_STATUS_DNAK;
	};

	// Disable master transfer
	twi->cr = AVR32_TWIM_CR_MDIS_MASK;

	return TWI_STATUS_OK;
}

/*
//Read one byte from slave
uint8_t TwiReadSingleByte(volatile avr32_twim_t *twi, uint8_t SlaveAddress)
{
	uint8_t byte;
	
	//Store slave address
	TwiSetSlaveAddress(twi, SlaveAddress);
	
	//Read mode
	TwiSetDirection(twi, 1);
	
	//Set start and stop bit at once -> only one byte will be read
	twi->cr |= AVR32_TWIM_CR_STOP_MASK | AVR32_TWIM_CR_START_MASK;
	
	//Wait for byte received
	while((TwiGetStatus(twi) & TWI_STATUS_RXRDY) == 0);
	
	//Get received byte
	byte = TwiGetRecByte(twi);
	
	//Wait for stop sent
	while((TwiGetStatus(twi) & TWI_STATUS_TXCOMP) == 0);
	
	return byte;
}*/


//Read multiple bytes from slave
uint32_t TwiReadBytes(volatile avr32_twim_t *twi, uint8_t SlaveAddress, uint8_t num, uint8_t *bytes)
{
	uint8_t i;
	
	//Reset TWI master to clear RHR and SR register
	twi->cr = AVR32_TWIM_CR_MEN_MASK;
	twi->cr = AVR32_TWIM_CR_SWRST_MASK;
	twi->scr = 0xFFFFFFFF;
	twi->cr = AVR32_TWIM_CR_MDIS_MASK;
	
	//Set read command
	twi->cmdr = (SlaveAddress << AVR32_TWIM_CMDR_SADR_OFFSET) | 
						(num << AVR32_TWIM_CMDR_NBYTES_OFFSET) |
						(AVR32_TWIM_CMDR_VALID_MASK) |
						(AVR32_TWIM_CMDR_START_MASK) |
						(AVR32_TWIM_CMDR_STOP_MASK) | 
						(AVR32_TWIM_CMDR_READ_MASK);

	//Enable TWI master
	twi->cr = AVR32_TWIM_CR_MEN_MASK;

	for(i=0; i<num; i++)
	{
		//Wait for byte received
		while((twi->sr & AVR32_TWIM_SR_RXRDY_MASK) == 0)
		{
			//Check address fail flag and return
			if((twi->sr & AVR32_TWIM_SR_ANAK_MASK))
				return TWI_STATUS_ANAK;
			//Check data fail flag and return
			if((twi->sr & AVR32_TWIM_SR_DNAK_MASK))
				return TWI_STATUS_DNAK;
		}
		
		//Save data in RHR into array
		*bytes = twi->rhr;
		bytes++;
	}

	// Disable master transfer
	twi->cr = AVR32_TWIM_CR_MDIS_MASK;

	return TWI_STATUS_OK;
}


//First write then read some bytes
uint32_t TwiWriteReadBytes(volatile avr32_twim_t *twi, uint8_t SlaveAddress, uint8_t wNum, uint8_t *wBytes, uint8_t rNum, uint8_t *rBytes)
{
	uint8_t i;
	
	//Reset TWI master to clear THR and SR register
	twi->cr = AVR32_TWIM_CR_MEN_MASK;
	twi->cr = AVR32_TWIM_CR_SWRST_MASK;
	twi->scr = 0xFFFFFFFF;
	twi->cr = AVR32_TWIM_CR_MDIS_MASK;

	//Set TX command
	twi->cmdr = (SlaveAddress << AVR32_TWIM_CMDR_SADR_OFFSET) |
				(wNum << AVR32_TWIM_CMDR_NBYTES_OFFSET) |
				(AVR32_TWIM_CMDR_VALID_MASK) |
				(AVR32_TWIM_CMDR_START_MASK) |
				//(AVR32_TWIM_CMDR_STOP_MASK) |
				(0 << AVR32_TWIM_CMDR_READ);
	
	//Set RX command
	twi->ncmdr = 0;
	twi->ncmdr |= (SlaveAddress << AVR32_TWIM_CMDR_SADR_OFFSET) |
				(rNum << AVR32_TWIM_CMDR_NBYTES_OFFSET) |
				(AVR32_TWIM_CMDR_VALID_MASK) |
				(AVR32_TWIM_CMDR_START_MASK) |
				(AVR32_TWIM_CMDR_STOP_MASK) |
				(1 << AVR32_TWIM_CMDR_READ);
		
	//Enable TWI master
	twi->cr = AVR32_TWIM_CR_MEN_MASK;

	//Wait for data ready
	while((twi->sr & AVR32_TWIM_SR_TXRDY_MASK) == 0)
	{
		//Check address fail flag and return
		if((twi->sr & AVR32_TWIM_SR_ANAK_MASK))
		return TWI_STATUS_ANAK;
	}
	
	//Transmit data bytes
	for(i=0; i<wNum; i++)
	{
		//Wait for byte transmitted
		while((twi->sr & AVR32_TWIM_SR_TXRDY_MASK) == 0)
		{
			//Check address fail flag and return
			if((twi->sr & AVR32_TWIM_SR_ANAK_MASK))
				return TWI_STATUS_ANAK;
			//Check data fail flag and return
			if((twi->sr & AVR32_TWIM_SR_DNAK_MASK))
				return TWI_STATUS_DNAK;
		}

		//Send next byte
		TwiTransmitByte(twi, *wBytes++);
	}
	
	//Clear command complete flag so next command will be executed
	twi->scr = AVR32_TWIM_SR_CCOMP_MASK;
	
	//Read bytes
	for(i=0; i<rNum; i++)
	{
		//Wait for byte received
		while((twi->sr & AVR32_TWIM_SR_RXRDY_MASK) == 0)
		{
			//Check address fail flag and return
			if((twi->sr & AVR32_TWIM_SR_ANAK_MASK))
				return TWI_STATUS_ANAK;
			//Check data fail flag and return
			if((twi->sr & AVR32_TWIM_SR_DNAK_MASK))
				return TWI_STATUS_DNAK;
		}
				
		//Save data in RHR into array
		*rBytes = twi->rhr;
		rBytes++;
	}

	// Disable master transfer
	twi->cr = AVR32_TWIM_CR_MDIS_MASK;

	return TWI_STATUS_OK;
	
		
	//Clear all status bits
	TwiClearStatus(twi);
	
	//Clear RHR register
	(void)TwiGetRecByte(twi);	
	
	//Setup transmission of data first---------------------
	//Store slave address
	TwiSetSlaveAddress(twi, SlaveAddress);	
	//Write mode
	TwiSetDirection(twi, 0);	
	//Set number of data bytes to write
	twi->cmdr |= (wNum<<AVR32_TWIM_CMDR_NBYTES);
	
	
	//Setup read of data in next command register---------
	//Store slave address
	twi->ncmdr &= ~AVR32_TWIM_NCMDR_SADR_MASK;
	twi->ncmdr |= (SlaveAddress<<AVR32_TWIM_NCMDR_SADR);
	//Set read mode
	twi->ncmdr &= ~AVR32_TWIM_NCMDR_READ_MASK;		//Clear bit
	//Set number of data bytes to read
	twi->ncmdr |= (rNum<<AVR32_TWIM_NCMDR_NBYTES);
	//Set START & STOP bit
	twi->ncmdr |= AVR32_TWIM_NCMDR_START_MASK | AVR32_TWIM_NCMDR_STOP_MASK;
	
	//Start transmission
	TwiSendStart(twi);
	
	for(i=0; i<wNum; i++)
	{
		//Wait for byte transmitted
		while((TwiGetStatus(twi) & TWI_STATUS_TXRDY) == 0);

		TwiTransmitByte(twi, *wBytes++);
	}	
	
	for(i=0; i<rNum; i++)
	{
		//Wait for byte transmitted
		while((TwiGetStatus(twi) & TWI_STATUS_RXRDY) == 0);

		TwiTransmitByte(twi, *rBytes++);
	}
	
	//Wait for stop sent
	while((TwiGetStatus(twi) & TWI_STATUS_CCOMP) == 0);	
}