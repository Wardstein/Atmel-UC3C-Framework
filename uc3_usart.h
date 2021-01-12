/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	Datei:	uc3_usart.h																	|
|	Beschreibung: Headerdatei für AVR32 UC3C											|
|																						|
|																						|
|	Controller: AT32UC3Cxxxx															|
|																						|
+---------------------------------------------------------------------------------------+
*/
#ifndef UC3_USART_H_
#define UC3_USART_H_

#include <stdint.h>

#define USART_PARITY_EVEN		0
#define USART_PARITY_ODD		1
#define USART_PARITY_FORCE0		2
#define USART_PARITY_FORCE1		3
#define USART_PARITY_NONE		4
#define USART_PARITY_MULTI		6

#define USART_STOPBITS_ONE		0
#define USART_STOPBITS_ONEHALF	1
#define USART_STOPBITS_TWO		2

#define USART_MODE_NORMAL		0
#define USART_MODE_ECHO			1
#define USART_MODE_LLOOP		2
#define USART_MODE_RLOOP		3

#define USART_LENGTH_5			0
#define USART_LENGTH_6			1
#define USART_LENGTH_7			2
#define USART_LENGTH_8			3
#define USART_LENGTH_9			4

#define USART_INT_RXRDY			0
#define USART_INT_TXRDY			1
#define USART_INT_RXBRK			2
#define USART_INT_OVRE			5
#define USART_INT_FRAME			6
#define USART_INT_PARE			7
#define USART_INT_TIMEOUT		8
#define USART_INT_TXEMPTY		9
#define USART_INT_ITER			10
#define USART_INT_RXBUFF		12
#define USART_INT_NACK			13
#define USART_INT_RIIC			16
#define USART_INT_DSRIC			17
#define USART_INT_DCDIC			18
#define USART_INT_CTSIC			19
#define USART_INT_MANE			20

	
void UsartDisable(volatile avr32_usart_t *usart);											//Disables (switches off) the usart
void UsartInitRs232(volatile avr32_usart_t *usart, uint32_t baudrate, uint8_t databits, uint8_t stopbits, uint8_t parity, uint32_t clockF);	//Setup the usart for rs232 (normal serial port) mode
void UsartSetMode(volatile avr32_usart_t *usart, uint8_t mode);								//Sets the mode of the usart (normal, echo, local loopback, remote loopback)
void UsartSetBaudrate(volatile avr32_usart_t *usart, uint32_t baudrate, uint32_t pba_hz);	//Set the usart baudrate
void UsartEnableInterrupt(volatile avr32_usart_t *usart, uint8_t bit);						//Enable usart interrupt
void UsartDisableInterrupt(volatile avr32_usart_t *usart, uint8_t bit);						//Disable usart interrupt
uint32_t UsartGetInterruptMask(volatile avr32_usart_t *usart);								//Get the interrupt mask register content (determines which interrupts are enabled)
uint32_t UsartGetStatus(volatile avr32_usart_t *usart);										//Get the channel status register
void UsartSendChar(volatile avr32_usart_t *usart, uint16_t character, uint8_t wait);		//Send character with usart
uint16_t UsartGetChar(volatile avr32_usart_t *usart);										//Get the last received character
void UsartSendString(volatile avr32_usart_t *usart, uint8_t *str);							//Send a string
uint8_t UsartTxStatus(volatile avr32_usart_t *usart);										//Get usart transmit status

#endif