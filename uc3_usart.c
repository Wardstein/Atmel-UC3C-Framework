/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	Datei:	uc3_usart.c																	|
|	Beschreibung: Codedatei für AVR32 UC3C												|
|																						|
|																						|
|	Controller: AT32UC3Cxxxx															|
|																						|
+---------------------------------------------------------------------------------------+
*/

#include <avr32/io.h>
#include "uc3_usart.h"

//Disables (switches off) the usart
void UsartDisable(volatile avr32_usart_t *usart)
{
	usart->cr |= (1<<AVR32_USART_CR_TXDIS) | (1<<AVR32_USART_CR_RXDIS);
	
	//Reset registers
	usart->mr = 0;
	usart->rtor = 0;
	usart->ttgr = 0;
}


//Setup the usart for rs232 (normal serial port) mode
//baudrate = serial baudrate (e.g. 19200)
//stopbits: 1 / 1.5 / 2
//parity: Usart parity (0: none; 1: odd; 2: even)
void UsartInitRs232(volatile avr32_usart_t *usart, uint32_t baudrate, uint8_t databits, uint8_t stopbits, uint8_t parity, uint32_t clockF)
{
	//Reset and shutdown usart
	UsartDisable(usart);
	
	//Set baudrate
	UsartSetBaudrate(usart, baudrate, clockF);
	
	//Set databits mode
	if(databits == USART_LENGTH_9)
		usart->mr |= (1<<AVR32_USART_MR_MODE9);
	else
		usart->mr |= (databits<<AVR32_USART_MR_CHRL);
	
	//Set stopbits
	usart->mr |= (stopbits<<AVR32_USART_MR_NBSTOP);
		
	//Set parity
	usart->mr |= (parity<<AVR32_USART_MR_PAR);
	
	//Enable transmitter & receiver
	usart->cr |= (1<<AVR32_USART_CR_TXEN) | (1<<AVR32_USART_CR_RXEN);
}


//Sets the mode of the usart (normal, echo, local loopback, remote loopback)
void UsartSetMode(volatile avr32_usart_t *usart, uint8_t mode)
{
	//Clear mode bits
	usart->mr &= ~AVR32_USART_MR_CHMODE_MASK;

	if(mode != USART_MODE_NORMAL)
		usart->mr |= (mode<<AVR32_USART_MR_CHMODE);
}


//Set the usart baudrate
//Source: Atmel Software Framework - USART driver - usart.c (25.12.2014)
void UsartSetBaudrate(volatile avr32_usart_t *usart, uint32_t baudrate, uint32_t pba_hz)
{
	unsigned int over = (pba_hz >= 16 * baudrate) ? 16 : 8;
	unsigned int cd = pba_hz / (over * baudrate);
	unsigned int fp = ((1 << AVR32_USART_BRGR_FP_SIZE) * pba_hz + (over * baudrate) / 2) / (over * baudrate) - (1 << AVR32_USART_BRGR_FP_SIZE) * cd;
	
	//if (cd < 1 || cd > (1 << AVR32_USART_BRGR_CD_SIZE) - 1)
	//	return USART_INVALID_INPUT;
	
	usart->mr = (usart->mr & ~(AVR32_USART_MR_USCLKS_MASK | 
								AVR32_USART_MR_SYNC_MASK | 
								AVR32_USART_MR_OVER_MASK)) |
								AVR32_USART_MR_USCLKS_MCK << AVR32_USART_MR_USCLKS_OFFSET |
								((over == 16) ? AVR32_USART_MR_OVER_X16 : AVR32_USART_MR_OVER_X8) << AVR32_USART_MR_OVER_OFFSET;
	
	usart->brgr = (cd << AVR32_USART_BRGR_CD_OFFSET) | (fp << AVR32_USART_BRGR_FP_OFFSET);
}


//Enable usart interrupt
//For bit number please referee to the data sheet
//Constants USART_INT_... contains the bit numbers
void UsartEnableInterrupt(volatile avr32_usart_t *usart, uint8_t bit)
{
	usart->ier |= (1<<bit);
}


//Disable usart interrupt
//For bit number please referee to the data sheet
//Constants USART_INT_... contains the bit numbers
void UsartDisableInterrupt(volatile avr32_usart_t *usart, uint8_t bit)
{
	usart->idr |= (1<<bit);
}


//Get the interrupt mask register content (determines which interrupts are enabled)
uint32_t UsartGetInterruptMask(volatile avr32_usart_t *usart)
{
	return usart->imr;
}


//Get the channel status register
//For different bits and their meanings please refer to the data sheet
uint32_t UsartGetStatus(volatile avr32_usart_t *usart)
{
	return usart->csr;
}


//Send character with usart
void UsartSendChar(volatile avr32_usart_t *usart, uint16_t character, uint8_t wait)
{
	usart->thr = character;
	
	if(wait == 1)
	{
		while(UsartTxStatus(usart) == 0);
	}
}


//Get the last received character
uint16_t UsartGetChar(volatile avr32_usart_t *usart)
{
	return usart->rhr & 0xFF;
}


//Send a string
void UsartSendString(volatile avr32_usart_t *usart, uint8_t *str)
{
	while(*str != 0)
		UsartSendChar(usart, *str++, 1);
}


//Get usart transmit status
//Returns 1 when transmit is finished, otherwise 0
uint8_t UsartTxStatus(volatile avr32_usart_t *usart)
{
	return (usart->csr & AVR32_USART_CSR_TXRDY_MASK) != 0;
}