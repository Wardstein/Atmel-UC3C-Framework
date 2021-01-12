/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	Datei:	uc3_eic.h																	|
|	Beschreibung: Headerdatei für AVR32 UC3C											|
|																						|
|																						|
|	Controller: AT32UC3Cxxxx															|
|																						|
+---------------------------------------------------------------------------------------+
*/
#ifndef UC3_EIC_H_
#define UC3_EIC_H_

#include <stdint.h>

#define EIC_MODE_EDGE		0
#define EIC_MODE_LEVEL		1

#define EIC_EDGE_FALLING	0
#define EIC_EDGE_RISING		1

#define EIC_LEVEL_LOW		0
#define EIC_LEVEL_HIGH		1

#define EIC_FILTER_OFF		0
#define EIC_FILTER_ON		1

#define EIC_INT_SYNC		0
#define EIC_INT_ASYNC		1


//Function prototypes
void EnableInterrupt(uint8_t IntNumber);
void DisableInterrupt(uint8_t IntNumber);
uint32_t GetInterruptMask(void);
uint32_t GetInterruptStatus(void);
void ClearInterrupt(uint8_t IntNumber);
void SetInterruptMode(uint8_t IntNumber, uint8_t Mode);
void SetInterruptEdgeMode(uint8_t IntNumber, uint8_t Mode);
void SetInterruptLevelMode(uint8_t IntNumber, uint8_t Mode);
void SetInterruptFilter(uint8_t IntNumber, uint8_t Status);
void SetInterruptSyncMode(uint8_t IntNumber, uint8_t Mode);
void EnableExtInterruptPin(uint8_t Number);
void DisableExtInterruptPin(uint8_t Number);
uint32_t GetExtInterruptStatus(void);



#endif