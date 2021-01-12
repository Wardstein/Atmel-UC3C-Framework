/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	Datei:	uc3_eic.c																	|
|	Beschreibung: Codedatei für AVR32 UC3C												|
|																						|
|																						|
|	Controller: AT32UC3Cxxxx															|
|																						|
+---------------------------------------------------------------------------------------+
*/

#include <avr32/io.h>
#include "uc3_eic.h"

//Enable Interrupt in Interrupt Enable Register
//0-7 is external Interrupt 0-7; 8 is NMI
void EnableInterrupt(uint8_t IntNumber)
{
	AVR32_EIC.ier = (1<<IntNumber);
}


//Disable Interrupt in Interrupt disable Register
//0-7 is external Interrupt 0-7; 8 is NMI
void DisableInterrupt(uint8_t IntNumber)
{
	AVR32_EIC.idr = (1<<IntNumber);
}


//Get interrupt mask
uint32_t GetInterruptMask(void)
{
	return AVR32_EIC.imr;
}


//Get the actual interrupt status
uint32_t GetInterruptStatus(void)
{
	return AVR32_EIC.isr;
}


//Clear Interrupt
//0-7 is external Interrupt 0-7; 8 is NMI
void ClearInterrupt(uint8_t IntNumber)
{
	AVR32_EIC.icr = (1<<IntNumber);
}


//Set interrupt mode (edge / level)
//0-7 is external Interrupt 0-7; 8 is NMI
void SetInterruptMode(uint8_t IntNumber, uint8_t Mode)
{
	if(Mode == EIC_MODE_EDGE)
		AVR32_EIC.mode &= ~(1<<IntNumber);
	else if(Mode == EIC_MODE_LEVEL)
		AVR32_EIC.mode |= (1<<IntNumber);
}


//Set edge mode (falling / rising)
//0-7 is external Interrupt 0-7; 8 is NMI
void SetInterruptEdgeMode(uint8_t IntNumber, uint8_t Mode)
{
	if(Mode == EIC_EDGE_FALLING)
		AVR32_EIC.edge &= ~(1<<IntNumber);
	else if(Mode == EIC_EDGE_RISING)
		AVR32_EIC.edge |= (1<<IntNumber);
}


//Set level mode (low / high)
//0-7 is external Interrupt 0-7; 8 is NMI
void SetInterruptLevelMode(uint8_t IntNumber, uint8_t Mode)
{
	if(Mode == EIC_LEVEL_LOW)
		AVR32_EIC.level &= ~(1<<IntNumber);
	else if(Mode == EIC_LEVEL_HIGH)
		AVR32_EIC.level |= (1<<IntNumber);
}


//Set filter mode
//0-7 is external Interrupt 0-7; 8 is NMI
void SetInterruptFilter(uint8_t IntNumber, uint8_t Status)
{
	if(Status == EIC_FILTER_OFF)
		AVR32_EIC.filter &= ~(1<<IntNumber);
	else if(Status == EIC_FILTER_ON)
		AVR32_EIC.filter |= (1<<IntNumber);
}


//Set interrupt syncronous mode
//0-7 is external Interrupt 0-7; 8 is NMI
void SetInterruptSyncMode(uint8_t IntNumber, uint8_t Mode)
{
	if(Mode == EIC_INT_SYNC)
		AVR32_EIC.async &= ~(1<<IntNumber);
	else if(Mode == EIC_INT_ASYNC)
		AVR32_EIC.async |= (1<<IntNumber);
}

//Enable external interrupt pin
//0-7 is external Interrupt 0-7; 8 is NMI
void EnableExtInterruptPin(uint8_t Number)
{
	AVR32_EIC.en |= (1<<Number);
}

//Disable external interrupt pin
//0-7 is external Interrupt 0-7; 8 is NMI
void DisableExtInterruptPin(uint8_t Number)
{
	AVR32_EIC.dis |= (1<<Number);
}

//Returns the status of the external interrupt pins
uint32_t GetExtInterruptStatus(void)
{
	return AVR32_EIC.ctrl;
}
