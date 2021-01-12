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

#include <avr32/io.h>
#include "uc3_cpu.h"

//Set controller clock frequency
//Clock = Frequency of the controller to run with [MHz]	e.g. 60MHz
//Crystal = Frequency of the crystal at oscillator 0 inputs [MHz] e.g. 12MHz
void CpuSetClockExtCrystal(uint32_t Clock, uint32_t Crystal)
{
	//Enable oscillator0 with crystal and automatic gain, startup time 2048 clock cycles
	AVR32_SCIF.unlock = 0xAA000000 | 0x0024;	//Unlock OSCCTRL0 register
	AVR32_SCIF.oscctrl[0] = AVR32_SCIF_OSCCTRL_OSCEN_MASK | AVR32_SCIF_OSCCTRL0_STARTUP_2048_RCOSC | AVR32_SCIF_OSCCTRL_AGC_MASK | (2<<AVR32_SCIF_OSCCTRL_GAIN_MASK) | AVR32_SCIF_OSCCTRL_MODE_CRYSTAL;
		
	//Wait for oscillator0 ready
	while((AVR32_SCIF.pclksr & AVR32_SCIF_PCLKSR_OSC0RDY_MASK) == 0);
	
	//Set clock source to oscillator0
	//AVR32_PM.unlock = 0xAA000000 | 0x0000;		//Unlock PLL0 register
	//AVR32_PM.mcctrl = AVR32_PM_MCSEL_OSC0;
		
	//Enable PLL with VCO frequency 80MHz < fvco < 180MHz, wide bandwidth mode
	//fvco = ((PLLMUL+1) / PLLDIV) * fosc
	int div = 1;
	int mul = (Clock/Crystal) - 1;
	AVR32_SCIF.unlock = 0xAA000000 | 0x001C;		//Unlock PLL0 register
	AVR32_SCIF.pll[0] = (mul<<AVR32_SCIF_PLL_PLLMUL) | (div<<AVR32_SCIF_PLL_PLLDIV) | AVR32_SCIF_PLL_PLLEN_MASK;
	
	//Wait for PLL lock
	while((AVR32_SCIF.pclksr & AVR32_SCIF_PCLKSR_PLL0_LOCK_MASK) == 0);

	/*
	AVR32_PM.unlock = 0xAA000000 | 0x0004;
	AVR32_PM.cpusel = 0x80;
	AVR32_PM.unlock = 0xAA000000 | 0x000C;
	AVR32_PM.pbasel = 0x80;
	AVR32_PM.unlock = 0xAA000000 | 0x0010;
	AVR32_PM.pbbsel = 0x80;
	AVR32_PM.unlock = 0xAA000000 | 0x0014;
	AVR32_PM.pbcsel = 0x80;
	*/
	
	//Select flash clock (flash clock max. 30MHz)
	if(Clock > 30)
		AVR32_FLASHC.fcr = AVR32_FLASHC_FCR_FWS_MASK;
		
	//Set PLL0 as main clock
	AVR32_PM.unlock = 0xAA000000 | 0x0000;		//Unlock PLL0 register
	AVR32_PM.mcctrl = AVR32_PM_MCSEL_PLL0;
}


//Set the clock source for a generic clock
void CpuSetGclkSource(uint8_t Gclk, uint8_t Source)
{
	AVR32_SCIF.gcctrl[Gclk] = (Source<<AVR32_SCIF_GCCTRL_OSCSEL) | AVR32_SCIF_GCCTRL_CEN_MASK;
}
