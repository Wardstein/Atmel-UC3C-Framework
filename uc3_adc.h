/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	Datei:	uc3_adc.h																	|
|	Beschreibung: Headerdatei für AVR32 UC3C											|
|																						|
|																						|
|	Controller: AT32UC3Cxxxx															|
|																						|
+---------------------------------------------------------------------------------------+
*/
#ifndef UC3_ADC_H_
#define UC3_ADC_H_

#include <stdint.h>

#define ADC_PRESCAL(ADCclock, fClockAdc)		(((fClockAdc)/((ADCclock)*2))-1)

#define ADC_SEQ_SINGLE			1
#define ADC_SEQ_DUAL			0

#define ADC_REF_1V				0
#define ADC_REF_06VCC			1
#define ADC_REF_EXT0			2
#define ADC_REF_EXT1			3

#define ADC_FREE_EN				1
#define ADC_FREE_DIS			0

#define ADC_MUXSETTLE_05		0
#define ADC_MUXSETTLE_15		1

#define ADC_SH_DIS				1
#define ADC_SH_EN				0

#define ADC_RES_8				2
#define ADC_RES_10				1
#define ADC_RES_12				0

#define ADC_TRG_SOFT			0
#define ADC_TRG_ADCIMER			1
#define ADC_TRG_EVENTCONTR		2
#define ADC_TRG_CONT			3

#define ADC_SHDYN_DIS			0
#define ADC_SHDYN_EN			1

#define ADC_OVSMPL_EN			1
#define ADC_OVSMPL_DIS			0

#define ADC_SOC_SINGLE			1
#define ADC_SOC_SEQ				0

#define ADC_SEQ_INP_ADCIN0		0
#define ADC_SEQ_INP_ADCIN1		1
#define ADC_SEQ_INP_ADCIN2		2
#define ADC_SEQ_INP_ADCIN3		3
#define ADC_SEQ_INP_ADCIN4		4
#define ADC_SEQ_INP_ADCIN5		5
#define ADC_SEQ_INP_ADCIN6		6
#define ADC_SEQ_INP_ADCIN7		7
#define ADC_SEQ_INP_DAC0		8
#define ADC_SEQ_INP_GND			10

#define ADC_SEQ_INN_ADCIN8		0
#define ADC_SEQ_INN_ADCIN9		1
#define ADC_SEQ_INN_ADCIN10		2
#define ADC_SEQ_INN_ADCIN11		3
#define ADC_SEQ_INN_ADCIN12		4
#define ADC_SEQ_INN_ADCIN13		5
#define ADC_SEQ_INN_ADCIN14		6
#define ADC_SEQ_INN_ADCIN15		7
#define ADC_SEQ_INN_DAC1		8
#define ADC_SEQ_INN_GND			9

#define ADC_INT_SEOS0			0
#define ADC_INT_SEOC0			1
#define ADC_INT_OVR0			2
#define ADC_INT_LOVR0			3
#define ADC_INT_SEOS1			4
#define ADC_INT_SEOC1			5
#define ADC_INT_OVR1			6
#define ADC_INT_LOVR1			7
#define ADC_INT_WM0				10
#define ADC_INT_WM1				11
#define ADC_INT_MSOC0			12
#define ADC_INT_MSOC1			13
#define ADC_INT_SUTD			14




void AdcStartSequence0(void);																		//Starts the ADC conversion process of sequencer 0
void AdcStartSequence1(void);																		//Starts the ADC conversion process of sequencer 1
void AdcSetup(uint8_t StartupTime, uint8_t MuxSettle, uint8_t EnSH, uint8_t Reference, uint8_t Freerunning, uint8_t SingleSequencer);		//Setup the ADC
void AdcConfigSequencer(uint8_t channel, uint8_t SeqConvs, uint8_t Resolution, uint8_t Trigger, uint8_t ShDyn, uint8_t Oversampling, uint8_t SoCsingle);	//Setup sequencer
void AdcSetOffsetValue(int8_t Offset);																// Set ADC offset register
void AdcSeqPosInputs(uint8_t channel, uint8_t inp1, uint8_t inp2, uint8_t inp3, uint8_t inp4, uint8_t inp5, uint8_t inp6, uint8_t inp7, uint8_t inp8);		//Setup positive input numbers of sequencer
void AdcSeqNegInputs(uint8_t channel, uint8_t inp1, uint8_t inp2, uint8_t inp3, uint8_t inp4, uint8_t inp5, uint8_t inp6, uint8_t inp7, uint8_t inp8);		//Setup negative input numbers of sequencer
void AdcSetPrescaler(uint16_t pre);																	//Set prescaler of ADC
uint32_t AdcGetStatus(void);																		//Get the adc status register
void AdcClearStatus(uint8_t bit);																	//Clear a statusbit
uint32_t AdcGetLastSeqResult(uint8_t channel);														//Get last conversion result of sequencer
void AdcEnableInterrupt(uint8_t BitNumber);															//Enable ADC interrupt
void AdcDisableInterrupt(uint8_t BitNumber);														//Disable ADC interrupt
uint32_t AdcGetInterruptMask(void);																	//Get ADC interrupt mask
int16_t AdcGetChannelResult(uint8_t channel);														//Get conversion result of channel

#endif