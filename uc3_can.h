/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	Datei:	uc3_can.h																	|
|	Beschreibung: Headerdatei für AVR32 UC3C											|
|																						|
|																						|
|	Controller: AT32UC3C2512															|
|																						|
+---------------------------------------------------------------------------------------+
*/
#ifndef UC3_CAN_H_
#define UC3_CAN_H_

#include <stdio.h>
#include <stdint.h>
#include <avr32/io.h>
#include <avr32/canif_110.h>

//Settings
#define USE_CHANNEL1					1



#define CANIF_SAMPLING_ONCE				0
#define CANIF_SAMPLING_THREE			1

#define CANIF_MODE_NORMAL				0
#define CANIF_MODE_LISTEN				1
#define CANIF_MODE_LOOPBACK				2

#define CANIF_OVERRUN_DIS				0
#define CANIF_OVERRUN_EN				1

#define CAN_SPEED_125K					0
#define CAN_SPEED_250K					1
#define CAN_SPEED_500K					2
#define CAN_SPEED_1M					3

#define CANIF_INT_BOFFIM				0
#define CANIF_INT_AERRIM				1
#define CANIF_INT_FERRIM				2
#define CANIF_INT_CERRIM				3
#define CANIF_INT_SERRIM				4
#define CANIF_INT_BERRIM				5
#define CANIF_INT_WKUPIM				6
#define CANIF_INT_RXOKIM				7
#define CANIF_INT_TXOKIM				8

#define CANIF_MOB_AUTO_TX				0
#define CANIF_MOB_AUTO_RX				1

#define CANIF_MOB_DIR_TX				1
#define CANIF_MOB_DIR_RX				0

#define CANIF_MOB_IRQ_EN				1
#define CANIF_MOB_IRQ_DIS				0


//The following values are calculated for fCANIF=48MHz
#define CAN_125K_PRES					23
#define CAN_250K_PRES					11
#define CAN_500K_PRES					5
#define CAN_1M_PRES						2
//The prescaler values above lead to 16 TQs per bit
//These 16 TQs minus 4 TQs have to be spread over the bit time
#define CAN_TQ_SJW						1
#define CAN_TQ_PRS						4
#define CAN_TQ_PHS1						4
#define CAN_TQ_PHS2						4
//With this configuration above the sampling point is
//located at the end of the second third of the bit time

typedef struct
{
	union
	{
		struct
		{
			uint32_t id			: 32;
			uint32_t id_mask	: 32;
		};
		struct
		{
			uint32_t			: 1;
			uint32_t rtr		: 1;
			uint32_t ide		: 1;
			uint32_t id			: 29;
			uint32_t			: 1;
			uint32_t rtr_mask	: 1;
			uint32_t ide_mask	: 1;
			uint32_t id_mask	: 29;
		};
	};
	uint8_t databytes[8];
} can_mobdata_t;

/*
typedef struct
{
	uint32_t ID;
	uint8_t databyte[8];
	uint8_t Length;
} can_message_t;
*/


//MOBCTRLn access
//Source: Atmel Software Framework - CANIF driver - canif.c (03.01.2015)
#define CANIF_get_mobctrl(ch,mob)            (((unsigned volatile long*)&(AVR32_CANIF.channel[ch].mobctrl))[mob*3])
#define CANIF_set_mobctrl(ch,mob,val)        (((unsigned volatile long*)&(AVR32_CANIF.channel[ch].mobctrl))[mob*3]=val)
	
//! MOBSR Register Access
//Source: Atmel Software Framework - CANIF driver - canif.c (03.01.2015)
#define CANIF_mob_get_status(ch,mob)         (((unsigned volatile long*) &(AVR32_CANIF.channel[ch].mobsr))[mob*3])
#define CANIF_mob_set_status(ch,mob,val)     {((unsigned volatile long*) &(AVR32_CANIF.channel[ch].mobscr))[mob*3]=val;}
#define CANIF_mob_clear_status(ch,mob)       {CANIF_mob_set_status(ch,mob,0x0F)}


void CanInit(uint8_t Channel, uint8_t Speed, uint8_t SamplingMethod, uint8_t ChannelMode, uint8_t OverrunMode);//, volatile can_mobdata_t *ramaddr);		//Initialize the CAN controller and channel
void CanSetRamAddress(uint8_t Channel, volatile uint32_t ramaddr);	//Set the address of the allocated RAM space for the MObs
uint8_t CanGetChannelStatus(uint8_t Channel);								//Get status register content
void CanEnableInterrupt(uint8_t Channel, uint8_t bit);						//Enable interrupt of the CAN controller
void CanDisableInterrupt(uint8_t Channel, uint8_t bit);						//Disable interrupt of the CAN controller
uint32_t CanGetInterruptMask(uint8_t Channel);								//Get CAN controller interrupt mask
void CanClearInterrupt(uint8_t Channel, uint8_t bit, uint8_t ClearMObNr);	//Clear Interrupt
uint32_t CanGetInterruptStatus(uint8_t Channel);							//Get interrupt status register
uint32_t CanSearchMOb(uint8_t Channel);										//Return CAN controller MOb search register
void CanEnableMOb(uint8_t Channel, uint8_t MOb, uint8_t EnableMObInterrupt);//Enable MOb (and if wanted it's interrupt) of CAN controller channel
void CanDisableMOb(uint8_t Channel, uint8_t MOb);							//Disable MOb (and it's interrupt) of CAN controller channel
void CanClearMObRxInterrupt(uint8_t Channel, uint8_t MOb);					//Clear MOb Rx interrupt status
uint32_t CanGetMObRxStatus(uint8_t Channel);								//Get MOb Rx interrupt status
void CanClearMObTxInterrupt(uint8_t Channel, uint8_t MOb);					//Clear MOb Tx interrupt status
uint32_t CanGetMObTxStatus(uint8_t Channel);								//Get MOb Tx interrupt status
void CanSetupMOb(uint8_t Channel, uint8_t MOb, uint8_t AutoMode, uint8_t Dir, uint8_t Length);					//Setup MOb
void CanSetMObMaskAndId(uint8_t Channel, uint8_t MOb, uint32_t id, uint8_t extendedId, uint32_t mask);			//Set MOb Mask and ID
void CanTransmitData(uint8_t Channel, uint8_t MOb, uint8_t Length, uint8_t *data, uint8_t enIrq);				//Transmit data via CAN bus
uint8_t CanGetLastTxMOb(uint8_t Channel);									//Get last transmit finished MOb
uint8_t CanGetLastRxMOb(uint8_t Channel);									//Get last receive finished MOb
uint8_t CanGetMObData(uint8_t Channel, uint8_t MOb, can_mobdata_t *mobData);//Get data of an MOb

#endif