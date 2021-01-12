/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	File:	uc3_twi.h																	|
|	Description: Codedatei für AVR32 UC3C												|
|																						|
|																						|
|	Controller: AT32UC3Cxxxx															|
|																						|
+---------------------------------------------------------------------------------------+
*/
#ifndef UC3_TWI_H_
#define UC3_TWI_H_

#include <stdint.h>
#include <avr32/twim_110.h>


#define TWI_INT_RXRDY			0
#define TWI_INT_TXRDY			1
#define TWI_INT_CRDY			2
#define TWI_INT_CCOMP			3
#define TWI_INT_IDLE			4
#define TWI_INT_BUSFREE			5
#define TWI_INT_ANAK			8
#define TWI_INT_DNAK			9
#define TWI_INT_ARBLST			10
#define TWI_INT_SMBALERT		11
#define TWI_INT_TOUT			12
#define TWI_INT_PECERR			13
#define TWI_INT_STOP			14

#define TWI_STATUS_OK			0

#define TWI_STATUS_RXRDY		(1<<0)
#define TWI_STATUS_TXRDY		(1<<1)
#define TWI_STATUS_CRDY			(1<<2)
#define TWI_STATUS_CCOMP		(1<<3)
#define TWI_STATUS_IDLE			(1<<4)
#define TWI_STATUS_BUSFREE		(1<<5)
#define TWI_STATUS_ANAK			(1<<8)
#define TWI_STATUS_DNAK			(1<<9)
#define TWI_STATUS_ARBLST		(1<<10)
#define TWI_STATUS_SMBALERT		(1<<11)
#define TWI_STATUS_TOUT			(1<<12)
#define TWI_STATUS_PECERR		(1<<13)
#define TWI_STATUS_STOP			(1<<14)
#define TWI_STATUS_MENB			(1<<16)


void TwiSetFrequency(volatile avr32_twim_t *twi, uint32_t speed, uint32_t pba_hz);							//Set the frequency of the TWI
void TwiInitMaster(volatile avr32_twim_t *twi, uint32_t Speed, uint32_t Clock);								//Initialize twi controller in master mode
void TwiSendStart(volatile avr32_twim_t *twi);																//Start the twi transmission
void TwiSendStop(volatile avr32_twim_t *twi);																//A stop condition will be transmitted right after the actual transmitted byte
void TwiSetSlaveAddress(volatile avr32_twim_t *twi, uint8_t SlaveAddress);									//Set address of the slave
void TwiSetDirection(volatile avr32_twim_t *twi, uint8_t mode);												//Set write (0) / read (1) mode
void TwiSetInternalAddress(volatile avr32_twim_t *twi, uint8_t bytes, uint32_t address);					//Set internal address
uint32_t TwiGetStatus(volatile avr32_twim_t *twi);															//Get the status register
void TwiEnableInterrupt(volatile avr32_twim_t *twi, uint8_t bit);											//Enable twi interrupt
void TwiDisableInterrupt(volatile avr32_twim_t *twi, uint8_t bit);											//Disable twi interrupt
uint32_t TwiGetInterruptMask(volatile avr32_twim_t *twi);													//Get interrupt mask register
uint8_t TwiGetRecByte(volatile avr32_twim_t *twi);															//Get the received byte
void TwiTransmitByte(volatile avr32_twim_t *twi, uint8_t byte);												//Write byte into transmit register
void TwiClearStatus(volatile avr32_twim_t *twi);															//Clear all status bits
//void TwiSendSingleByte(volatile avr32_twim_t *twi, uint8_t SlaveAddress, uint8_t byte);						//Transmit a single byte
//void TwiSendMultipleBytes(volatile avr32_twim_t *twi, uint8_t SlaveAddress, uint8_t num, uint8_t *bytes);	//Transmit multiple bytes
uint32_t TwiSendBytes(volatile avr32_twim_t *twi, uint8_t SlaveAddress, uint8_t nInternalAdress, uint8_t *InternalAddress, uint8_t num, uint8_t *bytes);
//uint8_t TwiReadSingleByte(volatile avr32_twim_t *twi, uint8_t SlaveAddress);								//Read one byte from slave
//void TwiReadMultipleBytes(volatile avr32_twim_t *twi, uint8_t SlaveAddress, uint8_t num, uint8_t *bytes);	//Read multiple bytes from slave
uint32_t TwiReadBytes(volatile avr32_twim_t *twi, uint8_t SlaveAddress, uint8_t num, uint8_t *bytes);
uint32_t TwiWriteReadBytes(volatile avr32_twim_t *twi, uint8_t SlaveAddress, uint8_t wNum, uint8_t *wBytes, uint8_t rNum, uint8_t *rBytes);

#endif