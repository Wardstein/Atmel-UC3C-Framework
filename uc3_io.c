/*
 * io.c
 *
 *  	Created on: 04.10.2012
 *      Author: 	Zippi
 *      File:		IO Driver
 *
 *      Beschreibung:
 *      	Inizialisiert die IO direkt für die IO funktion oder für die Freigabe der PIN Funktionen.
 *
 *      Beispiel:
 *			io_setup(AVR32_PIN_PB01,OUTPUT,PULLUP_DIS);		// Setzt den PB1 als Ausgang.
 *			io_setup(AVR32_PIN_PA13,INPUT,PULLUP_EN);			// Setzt den PA13 PIN auf INPUT und schalten den PULLUP ein.
 *			io_set(AVR32_PIN_PB01);							// Setzt den PB1 auf HIGH.
 *			io_clear(AVR32_PIN_PB01);							// Setzt den PB1 auf LOW.
 *			io_get(AVR32_PIN_PA13);							// Liest den akutellen Wert von PA13 ein und gibt in zurück.
 *
 *			io_setup(AVR32_PIN_PA28,FUNCTION_C,PULLUP_DIS);	// Gibt die FUNCTION_C am PA28 frei.
 */
#include <avr32/io.h>
#include "uc3_io.h"

void io_setup(unsigned int GPIO, unsigned int uiMode, unsigned int uiPullup)
{
	unsigned int uiPort = GPIO/32;
	unsigned int uiPin = GPIO%32;
	
	if(uiMode == OUTPUT)
	{
		AVR32_GPIO.port[uiPort].gpers = 1 << uiPin;			// Enable GPIO pin
		AVR32_GPIO.port[uiPort].oders = 1 << uiPin;			// Set GPIO -> output
	}
	else if(uiMode == INPUT)
	{
		AVR32_GPIO.port[uiPort].gpers = 1 << uiPin;			// Enable GPIO pin
		AVR32_GPIO.port[uiPort].oderc = 1 << uiPin;			// Set GPIO -> input

		io_pullup(GPIO,uiPullup);
	}
	else if(uiMode == FUNCTION_A)
	{
		AVR32_GPIO.port[uiPort].gperc = 1 << uiPin;			// Disable GPIO pin
		AVR32_GPIO.port[uiPort].oderc = 0 << uiPin;			// Set GPIO -> default
		AVR32_GPIO.port[uiPort].pmr0c = 1 << uiPin;			// Peripheral Multiplexer Select bits
		AVR32_GPIO.port[uiPort].pmr1c = 1 << uiPin;
		AVR32_GPIO.port[uiPort].pmr2c = 1 << uiPin;

		io_pullup(GPIO,uiPullup);
	}
	else if(uiMode == FUNCTION_B)
	{
		AVR32_GPIO.port[uiPort].gperc = 1 << uiPin;			// Disable GPIO pin
		AVR32_GPIO.port[uiPort].oderc = 0 << uiPin;			// Set GPIO -> default
		AVR32_GPIO.port[uiPort].pmr0s = 1 << uiPin;			// Peripheral Multiplexer Select bits
		AVR32_GPIO.port[uiPort].pmr1c = 1 << uiPin;
		AVR32_GPIO.port[uiPort].pmr2c = 1 << uiPin;

		io_pullup(GPIO,uiPullup);
	}
	else if(uiMode == FUNCTION_C)
	{
		AVR32_GPIO.port[uiPort].gperc = 1 << uiPin;			// Disable GPIO pin
		AVR32_GPIO.port[uiPort].oderc = 0 << uiPin;			// Set GPIO -> default
		AVR32_GPIO.port[uiPort].pmr0c = 1 << uiPin;			// Peripheral Multiplexer Select bits
		AVR32_GPIO.port[uiPort].pmr1s = 1 << uiPin;
		AVR32_GPIO.port[uiPort].pmr2c = 1 << uiPin;

		io_pullup(GPIO,uiPullup);
	}
	else if(uiMode == FUNCTION_D)
	{
		AVR32_GPIO.port[uiPort].gperc = 1 << uiPin;			// Disable GPIO pin
		AVR32_GPIO.port[uiPort].oderc = 0 << uiPin;			// Set GPIO -> default
		AVR32_GPIO.port[uiPort].pmr0s = 1 << uiPin;			// Peripheral Multiplexer Select bits
		AVR32_GPIO.port[uiPort].pmr1s = 1 << uiPin;
		AVR32_GPIO.port[uiPort].pmr2c = 1 << uiPin;

		io_pullup(GPIO,uiPullup);
	}
	else if(uiMode == FUNCTION_E)
	{
		AVR32_GPIO.port[uiPort].gperc = 1 << uiPin;			// Disable GPIO pin
		AVR32_GPIO.port[uiPort].oderc = 0 << uiPin;			// Set GPIO -> default
		AVR32_GPIO.port[uiPort].pmr0c = 1 << uiPin;			// Peripheral Multiplexer Select bits
		AVR32_GPIO.port[uiPort].pmr1c = 1 << uiPin;
		AVR32_GPIO.port[uiPort].pmr2s = 1 << uiPin;

		io_pullup(GPIO,uiPullup);
	}
	else if(uiMode == FUNCTION_F)
	{
		AVR32_GPIO.port[uiPort].gperc = 1 << uiPin;			// Disable GPIO pin
		AVR32_GPIO.port[uiPort].oderc = 0 << uiPin;			// Set GPIO -> default
		AVR32_GPIO.port[uiPort].pmr0s = 1 << uiPin;			// Peripheral Multiplexer Select bits
		AVR32_GPIO.port[uiPort].pmr1c = 1 << uiPin;
		AVR32_GPIO.port[uiPort].pmr2s = 1 << uiPin;

		io_pullup(GPIO,uiPullup);
	}
}

void io_pullup(unsigned int GPIO, unsigned int uiPullup)
{
	unsigned int uiPort = GPIO/32;
	unsigned int uiPin = GPIO%32;
	
	if(uiPullup == PULLUP_EN)
	{
		AVR32_GPIO.port[uiPort].puers = 1 << uiPin;
	}
	else if(uiPullup == PULLUP_DIS)
	{
		AVR32_GPIO.port[uiPort].puerc = 1 << uiPin;
	}
}

void io_set(unsigned int GPIO)
{
	unsigned int uiPort = GPIO/32;
	unsigned int uiPin = GPIO%32;
	
	AVR32_GPIO.port[uiPort].ovrs = 1 << uiPin;			// Set GPIO -> High
}

void io_clear(unsigned int GPIO)
{
	unsigned int uiPort = GPIO/32;
	unsigned int uiPin = GPIO%32;
	
	AVR32_GPIO.port[uiPort].ovrc = 1 << uiPin;			// Set GPIO -> LOW
}

void io_toggle(unsigned int GPIO)
{
	unsigned int uiPort = GPIO/32;
	unsigned int uiPin = GPIO%32;
	
	AVR32_GPIO.port[uiPort].ovrt = 1 << uiPin;
}

int io_get(unsigned int GPIO)
{
	unsigned int uiPort = GPIO/32;
	unsigned int uiPin = GPIO%32;
	
	if((AVR32_GPIO.port[uiPort].pvr & (1 << uiPin)) == 0)
	{
		return LOW;										// GPIO pin is LOW
	}

	return HIGH;										// GPIO pin is HIGH
}
