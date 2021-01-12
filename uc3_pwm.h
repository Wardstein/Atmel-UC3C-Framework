/*
 * io.h
 *
 *  	Created on: 25.04.2013
 *      Author: 	Zippi
 *      File:		PWM Driver
 */
#ifndef UC3_PWM_H_
#define UC3_PWM_H_


#define UPDATE_DUTY		0
#define UPDATE_MAX		1
#define START_LOW		0
#define START_HIGH		1
#define LEFT_ALIGNED	0
#define CENTER_ALIGNED	1

#define AVR32_PWM_CPRE_MCK_DIV_1		0
#define AVR32_PWM_CPRE_MCK_DIV_2		1
#define AVR32_PWM_CPRE_MCK_DIV_4		2
#define AVR32_PWM_CPRE_MCK_DIV_8		3
#define AVR32_PWM_CPRE_MCK_DIV_16		4
#define AVR32_PWM_CPRE_MCK_DIV_32		5
#define AVR32_PWM_CPRE_MCK_DIV_64		6
#define AVR32_PWM_CPRE_MCK_DIV_128		7
#define AVR32_PWM_CPRE_MCK_DIV_256		8
#define AVR32_PWM_CPRE_MCK_DIV_512		9
#define AVR32_PWM_CPRE_MCK_DIV_1024		10
#define AVR32_PWM_CPRE_MCK_DIV_CLKA		11
#define AVR32_PWM_CPRE_MCK_DIV_CLKB		12


void pwm_config(unsigned char uiChannel, char cPolarity, char cAlignment, unsigned char ucPrescaler);
void pwm_clk_pre(char cPrea, char cDiva, char cPreb, char cDivb);
unsigned char pwm_enable(unsigned char uiChannel);
unsigned char pwm_disable(unsigned char uiChannel);
void pwm_set_max(unsigned char uiChannel, unsigned int uiMax_value);
void pwm_set_next_max(unsigned char uiChannel, unsigned int uiNextMax_value);
void pwm_set_duty(unsigned char uiChannel, unsigned int uiDuty_value);
void pwm_set_next_duty(unsigned char uiChannel, unsigned int uiNext_value);


#endif /* UC3_PWM_H_ */