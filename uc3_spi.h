/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	File:	uc3_spi.h																	|
|	Description: Codedatei für AVR32 UC3x												|
|																						|
|																						|
|	Controller:																			|
|																						|
+---------------------------------------------------------------------------------------+
*/
#ifndef UC3_SPI_H_
#define UC3_SPI_H_

#include <stdint.h>
#include <avr32/spi_211.h>


#define SPI_INT_RDRF			0
#define SPI_INT_TDRE			1
#define SPI_INT_MODF			2
#define SPI_INT_OVRES			3
#define SPI_INT_NSSR			8
#define SPI_INT_TXEMPTY			9

#define SPI_STATUS_RDRF			(1<<0)
#define SPI_STATUS_TDRE			(1<<1)
#define SPI_STATUS_MODF			(1<<2)
#define SPI_STATUS_OVRS			(1<<3)
#define SPI_STATUS_MSSR			(1<<8)
#define SPI_STATUS_TXEMPTY		(1<<9)
#define SPI_STATUS_SPIENS		(1<<16)

#define SPI_BITS_8				0
#define SPI_BITS_9				1
#define SPI_BITS_10				2
#define SPI_BITS_11				3
#define SPI_BITS_12				4
#define SPI_BITS_13				5
#define SPI_BITS_14				6
#define SPI_BITS_15				7
#define SPI_BITS_16				8

#define	SPI_MODE_0				0		//Sample: leading edge, rising;		Shift: second edge, falling
#define SPI_MODE_1				1		//Sample: second edge, falling;		Shift: leading edge, rising
#define SPI_MODE_2				2		//Sample: leading edge, falling;	Shift: second edge, rising
#define SPI_MODE_3				3		//Sample: second edge, rising;		Shift: leading edge, falling


void SpiInitMaster(volatile avr32_spi_t *SpiInterface, uint8_t VariablePeripheralSelect, uint8_t FixedPeripheralSelectMask); //Initialize SPI controller in master mode
void SpiEnable(volatile avr32_spi_t *SpiInterface);																	//Enable SPI controller
void SpiDisable(volatile avr32_spi_t *SpiInterface);																//Disable SPI controller
int16_t getBaudDiv(const uint32_t baudrate, uint32_t pb_hz);														//Calculate divider for chip baudrate
void SpiSetChipSettings(volatile avr32_spi_t *SpiInterface, uint8_t Chip, uint8_t DataBits, uint8_t SpiMode, uint32_t Baudrate, uint32_t Clock);	//Set SPI controller settings for chosen slave chip
uint16_t SpiGetStatus(volatile avr32_spi_t *SpiInterface);															//Get the status register
uint8_t SpiSendRegisterEmpty(volatile avr32_spi_t *SpiInterface);													//Check if SPI controller send register is empty
void SpiEnableInterrupt(volatile avr32_spi_t *SpiInterface, uint8_t bit);											//Enable SPI interrupt
void SpiDisableInterrupt(volatile avr32_spi_t *SpiInterface, uint8_t bit);											//Disable SPI interrupt
uint32_t SpiGetInterruptMask(volatile avr32_spi_t *SpiInterface);													//Get interrupt mask register
uint16_t SpiGetRecByte(volatile avr32_spi_t *SpiInterface);															//Get the received value
void SpiWaitForTxFinished(volatile avr32_spi_t *SpiInterface);														//Wait for transmission finished
void SpiSendSingleByte(volatile avr32_spi_t *SpiInterface, uint16_t TxWord);										//Transmit a single byte
void SpiSendMultipleBytes(volatile avr32_spi_t *SpiInterface, uint16_t *TxWords, uint8_t num);						//Transmit multiple bytes
uint16_t SpiReadSingleByte(volatile avr32_spi_t *SpiInterface, uint16_t TxWord);									//Read one byte from slave
void SpiReadMultipleBytes(volatile avr32_spi_t *SpiInterface, uint16_t *RxWords, uint16_t *TxWords, uint8_t num);	//Read multiple bytes


//From ATMEL ASF compiler.h (11.10.2015)
/**
 * \brief Calculate \f$ \left\lceil \frac{a}{b} \right\rceil \f$ using
 * integer arithmetic.
 *
 * \param a An integer
 * \param b Another integer
 *
 * \return (\a a / \a b) rounded up to the nearest integer.
 */
#define div_ceil(a, b)	(((a) + (b) - 1) / (b))

#endif