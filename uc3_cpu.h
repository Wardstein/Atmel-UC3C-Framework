/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	Datei:	uc3_cpu.h																	|
|	Beschreibung: Headerdatei für AVR32 UC3C											|
|																						|
|																						|
|	Controller: AT32UC3Cxxxx															|
|																						|
+---------------------------------------------------------------------------------------+
*/

#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>

#define CPU_GCLK_SOURCE_RCSYS		0
#define CPU_GCLK_SOURCE_OSC32K		1
#define CPU_GCLK_SOURCE_RC8M		2
#define CPU_GCLK_SOURCE_OSC0		3
#define CPU_GCLK_SOURCE_OSC1		4
#define CPU_GCLK_SOURCE_PLL0		5
#define CPU_GCLK_SOURCE_PLL1		6
#define CPU_GCLK_SOURCE_CPU			7
#define CPU_GCLK_SOURCE_HSB			8
#define CPU_GCLK_SOURCE_PBA			9
#define CPU_GCLK_SOURCE_PBB			10
#define CPU_GCLK_SOURCE_PBC			11

#define CPU_GCLK_USBC				0
#define CPU_GCLK_CANIF				1
#define CPU_GCLK_AST				2
#define CPU_GCLK_PWM				4
#define CPU_GCLK_QDEC0				5
#define CPU_GCLK_QDEC1				6
#define CPU_GCLK_GCLKPIN0			9
#define CPU_GCLK_GCLKPIN1			10
#define CPU_GCLK_IISC				11


void CpuSetClockExtCrystal(uint32_t Clock, uint32_t Crystal);				//Set controller clock frequency
void CpuSetGclkSource(uint8_t Gclk, uint8_t Source);						//Set the clock source for a generic clock



#endif /* CPU_H_ */
