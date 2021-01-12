/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	Datei:	uc3_adc.c																	|
|	Beschreibung: Codedatei für AVR32 UC3C												|
|																						|
|																						|
|	Controller: AT32UC3Cxxxx															|
|																						|
+---------------------------------------------------------------------------------------+
*/

#include <avr32/io.h>
#include "uc3_adc.h"


//Starts the ADC conversion process of sequencer 0
void AdcStartSequence0(void)
{
	AVR32_ADCIFA.cr |= AVR32_ADCIFA_CR_SOC0_MASK;
}

//Starts the ADC conversion process of sequencer 1
void AdcStartSequence1(void)
{
	AVR32_ADCIFA.cr |= AVR32_ADCIFA_CR_SOC1_MASK;
}

//Setup the ADC
//StartupTime: Startup time value; Startup Time = (STARTUP+1) * 8 / ADCClock
//MuxSettle: Multiplexer settle time
//EnSH: Enable Sample & Hold
//Reference: Reference source
//Freerunning: Enable free running
//SingleSequencer: Enable single sequencer mode
void AdcSetup(uint8_t StartupTime, uint8_t MuxSettle, uint8_t EnSH, uint8_t Reference, uint8_t Freerunning, uint8_t SingleSequencer)
{
	AVR32_ADCIFA.cfg = (StartupTime<<AVR32_ADCIFA_CFG_SUT) |
						(MuxSettle<<AVR32_ADCIFA_CFG_MUXSET) |
						(EnSH<<AVR32_ADCIFA_CFG_SHD) |
						(Reference<<AVR32_ADCIFA_CFG_RS) |
						(Freerunning<<AVR32_ADCIFA_CFG_FRM) |
						(SingleSequencer<<AVR32_ADCIFA_CFG_SSMQ) |
						AVR32_ADCIFA_CFG_ADCEN_MASK;
}


//Setup the sequencer
//SeqConvs: Number of conversions n the sequence (this value + 1)
//Resolution: Resolution of this sequencer
//Trigger: Select the trigger source
//ShDyn: Enable Sample & Hold dynamic mode
//Oversampling: Oversample by 2
//SoCsingle: enable single conversion on start of conversion event
void AdcConfigSequencer(uint8_t channel, uint8_t SeqConvs, uint8_t Resolution, uint8_t Trigger, uint8_t ShDyn, uint8_t Oversampling, uint8_t SoCsingle)
{
	SeqConvs--;		//Data sheet: "The number of conversions to perform in the sequence is (CNVNB+1)."
	
	uint32_t value = (SeqConvs<<AVR32_ADCIFA_SEQCFG0_CNVNB) |
						(Resolution<<AVR32_ADCIFA_SEQCFG0_SRES) |
						(Trigger<<AVR32_ADCIFA_SEQCFG0_TRGSEL) |
						(ShDyn<<AVR32_ADCIFA_SEQCFG0_SHDYN) |
						(Oversampling<<AVR32_ADCIFA_SEQCFG0_OVSX2) |
						(SoCsingle<<AVR32_ADCIFA_SEQCFG0_SOCB);
						
	if(channel == 0)
		AVR32_ADCIFA.seqcfg0 = value;
	else if(channel == 1)
		AVR32_ADCIFA.seqcfg1 = value;
	
}

// Set the ADC offset
void AdcSetOffsetValue(int8_t Offset)
{
	AVR32_ADCIFA.adccal &= 0x7FFF;
	AVR32_ADCIFA.adccal |= ((Offset & 0x3F) << AVR32_ADCIFA_ADCCAL_OCAL_OFFSET);
}

//Setup positive input numbers of sequencer
void AdcSeqPosInputs(uint8_t channel, uint8_t inp1, uint8_t inp2, uint8_t inp3, uint8_t inp4, uint8_t inp5, uint8_t inp6, uint8_t inp7, uint8_t inp8)
{
	uint32_t value0 = (inp1<<AVR32_ADCIFA_INPSEL00_CNV0) |
						(inp2<<AVR32_ADCIFA_INPSEL00_CNV1) |
						(inp3<<AVR32_ADCIFA_INPSEL00_CNV2) |
						(inp4<<AVR32_ADCIFA_INPSEL00_CNV3);
	
	uint32_t value1 = (inp5<<AVR32_ADCIFA_INPSEL10_CNV4) |
						(inp6<<AVR32_ADCIFA_INPSEL10_CNV5) |
						(inp7<<AVR32_ADCIFA_INPSEL10_CNV6) |
						(inp8<<AVR32_ADCIFA_INPSEL10_CNV7);

	if(channel == 0)
	{
		AVR32_ADCIFA.inpsel00 = value0;
		AVR32_ADCIFA.inpsel10 = value1;
	}
	else if(channel == 1)
	{
		AVR32_ADCIFA.inpsel01 = value0;
		AVR32_ADCIFA.inpsel11 = value1;
	}
}


//Setup negative input numbers of sequencer
void AdcSeqNegInputs(uint8_t channel, uint8_t inp1, uint8_t inp2, uint8_t inp3, uint8_t inp4, uint8_t inp5, uint8_t inp6, uint8_t inp7, uint8_t inp8)
{
	uint32_t value0 = (inp1<<AVR32_ADCIFA_INNSEL00_CNV0) |
						(inp2<<AVR32_ADCIFA_INNSEL00_CNV1) |
						(inp3<<AVR32_ADCIFA_INNSEL00_CNV2) |
						(inp4<<AVR32_ADCIFA_INNSEL00_CNV3);
	
	uint32_t value1 = (inp5<<AVR32_ADCIFA_INNSEL10_CNV4) |
						(inp6<<AVR32_ADCIFA_INNSEL10_CNV5) |
						(inp7<<AVR32_ADCIFA_INNSEL10_CNV6) |
						(inp8<<AVR32_ADCIFA_INNSEL10_CNV7);

	if(channel == 0)
	{
		AVR32_ADCIFA.innsel00 = value0;
		AVR32_ADCIFA.innsel10 = value1;
	}
	else if(channel == 1)
	{
		AVR32_ADCIFA.innsel01 = value0;
		AVR32_ADCIFA.innsel11 = value1;
	}
}


//Set prescaler of ADC
void AdcSetPrescaler(uint16_t pre)
{
	AVR32_ADCIFA.ckdiv = pre;
}


//Get content of the ADC Status Register
uint32_t AdcGetStatus(void)
{
	return AVR32_ADCIFA.sr;
}


//Clear a statusbit
//Bitnumber refer to datasheet
void AdcClearStatus(uint8_t bit)
{
	AVR32_ADCIFA.scr = (1<<bit);
}


//Get last conversion result of sequencer
uint32_t AdcGetLastSeqResult(uint8_t channel)
{
	if(channel == 0)
		return AVR32_ADCIFA.lcv0;
	else
		return AVR32_ADCIFA.lcv1;
}


//Enable ADC Interrupt
//BitNumber: Refer data sheet for bit number of desired interrupt
void AdcEnableInterrupt(uint8_t BitNumber)
{
	AVR32_ADCIFA.ier |= (1<<BitNumber);
}

//Disable ADC Interrupt
//BitNumber: Refer data sheet for bit number of desired interrupt
void AdcDisableInterrupt(uint8_t BitNumber)
{
	AVR32_ADCIFA.idr |= (1<<BitNumber);
}


//Get ADC interrupt mask
uint32_t AdcGetInterruptMask(void)
{
	return AVR32_ADCIFA.imr;
}


//Get conversion result of channel
int16_t AdcGetChannelResult(uint8_t channel)
{
	return AVR32_ADCIFA.resx[channel];
}
