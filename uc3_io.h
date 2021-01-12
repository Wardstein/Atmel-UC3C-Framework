/*
 * io.h
 *
 *  	Created on: 22.09.2012
 *      Author: 	Zippi
 *      File:		IO Driver
 */
#ifndef IO_H_
#define IO_H_

#define LOW			0
#define HIGH		1

#define INPUT		1
#define OUTPUT		2
#define FUNCTION_A	3
#define FUNCTION_B	4
#define FUNCTION_C	5
#define FUNCTION_D	6
#define FUNCTION_E	7
#define FUNCTION_F	8
#define PULLUP_DIS	1
#define PULLUP_EN	2

void io_setup(unsigned int GPIO, unsigned int uiMode, unsigned int uiPullup);	// Configuriert den io Pin
void io_pullup(unsigned int GPIO, unsigned int uiPullup);						// Stellt den Pullup ein
void io_set(unsigned int GPIO);												// Setzt den pin auf High
void io_clear(unsigned int GPIO);												// Setzt den pin auf low
void io_toggle(unsigned int GPIO);												// Schaltet den status des pins um
int io_get(unsigned int GPIO);												// Liest den Inhalt vom pin und gibt eine 1 oder 0 zurück

#endif /* IO_H_ */
