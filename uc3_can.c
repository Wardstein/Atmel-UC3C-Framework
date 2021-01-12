/*
+---------------------------------------------------------------------------------------+
|																						|
|	Author: Jonas Eberhard | www.joseng.de												|
|	Datei:	uc3_can.c																	|
|	Beschreibung: Codedatei für AVR32 UC3C												|
|																						|
|																						|
|	Controller: AT32UC3C2512															|
|																						|
+---------------------------------------------------------------------------------------+
*/

#include <avr32/io.h>
#include "uc3_can.h"

#include "uc3_io.h"

volatile can_mobdata_t CanMObData0[16];					//MOb data allocated space in RAM
#if USE_CHANNEL1
volatile can_mobdata_t CanMObData1[16];					//MOb data allocated space in RAM
#endif

//Initialize the CAN controller and channel
void CanInit(uint8_t Channel, uint8_t Speed, uint8_t SamplingMethod, uint8_t ChannelMode, uint8_t OverrunMode)//, volatile can_mobdata_t *ramaddr)
{
	//Reset channel
	AVR32_CANIF.channel[Channel].canctrl = AVR32_CANIF_CANCTRL_INIT_MASK;
	//Wait for channel reset (CANSR.CES will be cleared)
	while((CanGetChannelStatus(Channel) & AVR32_CANIF_CANSR_CES_MASK) == 1);
	//Release INIT bit
	AVR32_CANIF.channel[Channel].canctrl = 0;
	
	//Set RAM address
	//CanSetRamAddress(Channel, ramaddr);
	if(Channel == 0)
		CanSetRamAddress(0, (uint32_t)&CanMObData0);
	#if USE_CHANNEL1
	else if(Channel == 1)
		CanSetRamAddress(1, (uint32_t)&CanMObData1);
	#endif
	
	//Set channel settings
	switch(Speed)
	{
		case CAN_SPEED_125K:
			AVR32_CANIF.channel[Channel].cancfg = (OverrunMode<<AVR32_CANIF_CANCFG_OVRM) |
													(ChannelMode<<AVR32_CANIF_CANCFG_CMODE) |
													(SamplingMethod<<AVR32_CANIF_CANCFG_SM) |
													(CAN_TQ_SJW<<AVR32_CANIF_CANCFG_SJW) |
													(CAN_TQ_PRS<AVR32_CANIF_CANCFG_PRS) |
													(CAN_TQ_PHS2<<AVR32_CANIF_CANCFG_PHS2) |
													(CAN_TQ_PHS1<<AVR32_CANIF_CANCFG_PHS1) |
													(CAN_125K_PRES<<AVR32_CANIF_CANCFG_PRES);
			break;
		
		case CAN_SPEED_250K:
			AVR32_CANIF.channel[Channel].cancfg = (OverrunMode<<AVR32_CANIF_CANCFG_OVRM) |
													(ChannelMode<<AVR32_CANIF_CANCFG_CMODE) |
													(SamplingMethod<<AVR32_CANIF_CANCFG_SM) |
													(CAN_TQ_SJW<<AVR32_CANIF_CANCFG_SJW) |
													(CAN_TQ_PRS<AVR32_CANIF_CANCFG_PRS) |
													(CAN_TQ_PHS2<<AVR32_CANIF_CANCFG_PHS2) |
													(CAN_TQ_PHS1<<AVR32_CANIF_CANCFG_PHS1) |
													(CAN_250K_PRES<<AVR32_CANIF_CANCFG_PRES);
			break;
		
		case CAN_SPEED_500K:
			AVR32_CANIF.channel[Channel].cancfg = (OverrunMode<<AVR32_CANIF_CANCFG_OVRM) |
													(ChannelMode<<AVR32_CANIF_CANCFG_CMODE) |
													(SamplingMethod<<AVR32_CANIF_CANCFG_SM) |
													(CAN_TQ_SJW<<AVR32_CANIF_CANCFG_SJW) |
													(CAN_TQ_PRS<AVR32_CANIF_CANCFG_PRS) |
													(CAN_TQ_PHS2<<AVR32_CANIF_CANCFG_PHS2) |
													(CAN_TQ_PHS1<<AVR32_CANIF_CANCFG_PHS1) |
													(CAN_500K_PRES<<AVR32_CANIF_CANCFG_PRES);
			break;
		
		case CAN_SPEED_1M:
			AVR32_CANIF.channel[Channel].cancfg = (OverrunMode<<AVR32_CANIF_CANCFG_OVRM) |
													(ChannelMode<<AVR32_CANIF_CANCFG_CMODE) |
													(SamplingMethod<<AVR32_CANIF_CANCFG_SM) |
													(CAN_TQ_SJW<<AVR32_CANIF_CANCFG_SJW) |
													(CAN_TQ_PRS<AVR32_CANIF_CANCFG_PRS) |
													(CAN_TQ_PHS2<<AVR32_CANIF_CANCFG_PHS2) |
													(CAN_TQ_PHS1<<AVR32_CANIF_CANCFG_PHS1) |
													(CAN_1M_PRES<<AVR32_CANIF_CANCFG_PRES);
			break;
		
		//Not known speed configuration -> return without enabling channel
		default:
			return;
			break;
	}
	
	//Enable channel
	AVR32_CANIF.channel[Channel].canctrl |= AVR32_CANIF_CANCTRL_CEN_MASK;
	
	//Wait for channel is ready
	while((CanGetChannelStatus(Channel) & AVR32_CANIF_CANSR_CES_MASK) == 0);
}


//Set the address of the allocated RAM space for the MObs
void CanSetRamAddress(uint8_t Channel, volatile uint32_t ramaddr)
{
	AVR32_CANIF.channel[Channel].canramb = ramaddr;
}


//Get status register content
uint8_t CanGetChannelStatus(uint8_t Channel)
{
	return AVR32_CANIF.channel[Channel].cansr;
}


//Enable interrupt of the CAN controller
//For bit numbers please refer datasheet
//The defines CANIF_INT_xxxxxx provide the bit numbers
void CanEnableInterrupt(uint8_t Channel, uint8_t bit)
{
	AVR32_CANIF.channel[Channel].canier = (1<<bit);
}


//Disable interrupt of the CAN controller
//For bit numbers please refer datasheet
//The defines CANIF_INT_xxxxxx provide the bit numbers
void CanDisableInterrupt(uint8_t Channel, uint8_t bit)
{
	AVR32_CANIF.channel[Channel].canidr = (1<<bit);
}


//Get CAN controller interrupt mask
uint32_t CanGetInterruptMask(uint8_t Channel)
{
	return AVR32_CANIF.channel[Channel].canimr;
}


//Clear Interrupt
void CanClearInterrupt(uint8_t Channel, uint8_t bit, uint8_t ClearMObNr)
{
	AVR32_CANIF.channel[Channel].caniscr = (1<<bit);
	
	if(ClearMObNr > 0)
		AVR32_CANIF.channel[Channel].caniscr = AVR32_CANIF_CANISCR_LSMOB_MASK;
}


//Get interrupt status register
uint32_t CanGetInterruptStatus(uint8_t Channel)
{
	return AVR32_CANIF.channel[Channel].canisr;
}


//Return CAN controller MOb search register
//Holds values for the highest priority TxOK, RxOK and Available MOb
uint32_t CanSearchMOb(uint8_t Channel)
{
	return AVR32_CANIF.channel[Channel].mobsch;
}


//Enable MOb (and if wanted it's interrupt) of CAN controller channel
void CanEnableMOb(uint8_t Channel, uint8_t MOb, uint8_t EnableMObInterrupt)
{
	AVR32_CANIF.channel[Channel].mober = (1<<MOb);
	
	if(EnableMObInterrupt > 0)
		AVR32_CANIF.channel[Channel].mobier = (1<<MOb);
}


//Disable MOb (and it's interrupt) of CAN controller channel
void CanDisableMOb(uint8_t Channel, uint8_t MOb)
{
	AVR32_CANIF.channel[Channel].mobdr = (1<<MOb);
	AVR32_CANIF.channel[Channel].mobidr = (1<<MOb);
}


//Clear MOb Rx interrupt status
void CanClearMObRxInterrupt(uint8_t Channel, uint8_t MOb)
{
	//CANIF_mob_set_status(Channel, MOb, 1);
	AVR32_CANIF.channel[Channel].mrxiscr = (1<<MOb);
}


//Get MOb Rx interrupt status
uint32_t CanGetMObRxStatus(uint8_t Channel)
{
	return AVR32_CANIF.channel[Channel].mrxisr;
}


//Clear MOb Tx interrupt status
void CanClearMObTxInterrupt(uint8_t Channel, uint8_t MOb)
{
	//CANIF_mob_set_status(Channel, MOb, 2);
	AVR32_CANIF.channel[Channel].mtxiscr = (1<<MOb);
}


//Get MOb Tx interrupt status
uint32_t CanGetMObTxStatus(uint8_t Channel)
{
	return AVR32_CANIF.channel[Channel].mtxisr;
}


//Setup MOb
void CanSetupMOb(uint8_t Channel, uint8_t MOb, uint8_t AutoMode, uint8_t Dir, uint8_t Length)
{
	if(Length > 8)
		Length = 8;
		
	CANIF_set_mobctrl(Channel, MOb, (AutoMode<<AVR32_CANIF_MOBCTRL_AM) |
												(Dir<<AVR32_CANIF_MOBCTRL_DIR) |
												(Length<<AVR32_CANIF_MOBCTRL_DLC));
}


//Set MOb Mask and ID
void CanSetMObMaskAndId(uint8_t Channel, uint8_t MOb, uint32_t id, uint8_t extendedId, uint32_t mask)
{	if(Channel == 0)
	{
		CanMObData0[MOb].id = id;
		CanMObData0[MOb].ide = extendedId;
		CanMObData0[MOb].id_mask = mask;
	}
	#if USE_CHANNEL1
	else if(Channel == 1)
	{
		CanMObData1[MOb].id = id;
		CanMObData1[MOb].ide = extendedId;
		CanMObData1[MOb].id_mask = mask;
	}
	#endif

}


//Transmit data via CAN bus
void CanTransmitData(uint8_t Channel, uint8_t MOb, uint8_t Length, uint8_t *data, uint8_t enIrq)
{
	uint8_t i;
	
	//Copy data from array to MOb buffer
	for(i=0; i<Length; i++)
	{
		if(Channel == 0)
			CanMObData0[MOb].databytes[i] = data[i];
		#if USE_CHANNEL1
		else if(Channel == 1)
			CanMObData1[MOb].databytes[i] = data[i];
		#endif
	}
	//Fill the remaining data bytes with zero
	for(; i<8; i++)
	{
		if(Channel == 0)
			CanMObData0[MOb].databytes[i] = 0;
		#if USE_CHANNEL1
		else if(Channel == 1)
			CanMObData1[MOb].databytes[i] = 0;
		#endif
	}
	
	//Setup MOb data
	CanSetupMOb(Channel, MOb, CANIF_MOB_AUTO_TX, CANIF_MOB_DIR_TX, Length);
	//Send data
	CanEnableMOb(Channel, MOb, enIrq);
}


//Get last transmit finished mode
uint8_t CanGetLastTxMOb(uint8_t Channel)
{
	return ((CanSearchMOb(Channel) & AVR32_CANIF_MOBSCH_MTXOK_MASK)>>AVR32_CANIF_MOBSCH_MTXOK);
}


//Get last receive finished MOb
uint8_t CanGetLastRxMOb(uint8_t Channel)
{
	return ((CanSearchMOb(Channel) & AVR32_CANIF_MOBSCH_MRXOK_MASK)>>AVR32_CANIF_MOBSCH_MRXOK);
}


//Get data of an MOb, return value is the length
uint8_t CanGetMObData(uint8_t Channel, uint8_t MOb, can_mobdata_t *mobData)
{
	if(Channel == 0)
	{
		*mobData = CanMObData0[MOb];
	}
	#if USE_CHANNEL1
	else if(Channel == 1)
	{
		*mobData = CanMObData1[MOb];
	}
	#endif

	return CANIF_get_mobctrl(Channel, MOb) & AVR32_CANIF_MOBCTRL_DLC_MASK;
}