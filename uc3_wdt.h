/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	Datei:	uc3_wdt.h																	|
|	Beschreibung: Headerdatei für AVR32 UC3C											|
|																						|
|																						|
|	Controller: AT32UC3Cxxxx															|
|																						|
+---------------------------------------------------------------------------------------+
*/
#ifndef UC3_WDT_H_
#define UC3_WDT_H_

#include <stdint.h>

void EnableWatchdog(uint8_t);				//Enable watchdog with selected prescaler
void DisableWatchdog(void);					//Disable watchdog operation
void ResetWatchdog(void);					//Reset watchdog


#endif