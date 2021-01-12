/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	Datei:	uc3_wdt.c																	|
|	Beschreibung: Codedatei für AVR32 UC3C												|
|																						|
|																						|
|	Controller: AT32UC3Cxxxx															|
|																						|
+---------------------------------------------------------------------------------------+
*/

#include <avr32/io.h>
#include "uc3_wdt.h"

//Enable Watchdog operation with selected prescaler
//Twdt = (2^(pre+1))/fRC
void EnableWatchdog(uint8_t Prescaler)
{
	AVR32_WDT.ctrl = 0x55000000 | (Prescaler << AVR32_WDT_PSEL) | AVR32_WDT_CTRL_CEN_MASK | 1;
	AVR32_WDT.ctrl = 0xAA000000 | (Prescaler << AVR32_WDT_PSEL) | AVR32_WDT_CTRL_CEN_MASK | 1;
}


//Disables the watchdog
void DisableWatchdog(void)
{
	AVR32_WDT.ctrl = 0x55000000;
	AVR32_WDT.ctrl = 0xAA000000;
}


//Reset watchdog
void ResetWatchdog(void)
{
	AVR32_WDT.clr = 0x55000001;
	AVR32_WDT.clr = 0xAA000001;
}
