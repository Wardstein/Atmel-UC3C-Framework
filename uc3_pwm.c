/*
 * uc3_pwm.c
 *
 *  	Created on: 25.04.2013 (30.12.2014)
 *      Author: 	Zippi  (adapted for AT32UC3C2512 by Jonas Eberhard)
 *      File:		PWM Driver
 *
 *      Beschreibung:
 *      	Inizialisiert die PWM Funktionen.
 *
 *      Beispiel (einfacher PWM für z.B. LEDs):
 *				pwm_config(0, UPDATE_DUTY, START_HIGH, LEFT_ALIGNED, 1);	// Configuriere PWM[0], Funktion pwm_set_next updatet das Duty Register, Startet mit High, CLK/2
 *				pwm_set_max(0, 0xffff);										// Setzt den maximalen Zählerwert auf 65535
 *				pwm_enable(0);												// Aktiviert PWM[0]
 *				pwm_set_next(0, 0x8000);									// Setzt das Duty Register auf 32768 (PWM mit 50% High, 50% Low)
 *
 */
#include <avr32/io.h>
#include "uc3_pwm.h"

void pwm_config(unsigned char uiChannel, char cPolarity, char cAlignment, unsigned char ucPrescaler)
{
	//Write protect is not enabled at startup of the controller, so we don't have to write to it
	
	AVR32_PWM.channel[uiChannel].cmr =	cPolarity << AVR32_PWM_CPOL_OFFSET | 	// Channel Polarity
										cAlignment << AVR32_PWM_CALG_OFFSET | 	// Channel Alignment
										ucPrescaler << AVR32_PWM_CPRE_OFFSET ; 	// Channel Pre-scaler
}

void pwm_clk_pre(char cPrea, char cDiva, char cPreb, char cDivb)
{
	AVR32_PWM.clk =	cPreb << AVR32_PWM_PREB_OFFSET | 	// PREB
					cDivb << AVR32_PWM_DIVB_OFFSET |	// DIVB: CLKB Divide Factor
					cPrea << AVR32_PWM_PREA_OFFSET |	// PREA
					cDiva << AVR32_PWM_DIVA_OFFSET ;	// DIVA: CLKA Divide Factor
}

unsigned char pwm_enable(unsigned char uiChannel)
{
	AVR32_PWM.ena =	1 << uiChannel ; 	// Enable Channel X

	if((AVR32_PWM.sr & (1 << uiChannel))==0)
		return 0;						// False
	
	return 1;							// True
}

unsigned char pwm_disable(unsigned char uiChannel)
{
	AVR32_PWM.dis =	1 << uiChannel ; 	// Enable Channel X

	if((AVR32_PWM.sr & (1 << uiChannel))==0)
		return 1;					// True
	
	return 0;						// False
}

void pwm_set_max(unsigned char uiChannel, unsigned int uiMax_value)
{
	//Write protect is not enabled at startup of the controller, so we don't have to write to it
	
	AVR32_PWM.channel[uiChannel].cprd = uiMax_value;
}

void pwm_set_next_max(unsigned char uiChannel, unsigned int uiNextMax_value)
{
	//Write protect is not enabled at startup of the controller, so we don't have to write to it
	
	AVR32_PWM.channel[uiChannel].cprdupd = uiNextMax_value;
}

void pwm_set_duty(unsigned char uiChannel, unsigned int uiDuty_value)
{
	AVR32_PWM.channel[uiChannel].cdty = uiDuty_value;
}

void pwm_set_next_duty(unsigned char uiChannel, unsigned int uiNext_value)
{
	AVR32_PWM.channel[uiChannel].cdtyupd = uiNext_value;
}