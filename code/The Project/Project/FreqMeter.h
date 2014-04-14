/*----------------------------------------------------------------------------
 * Name:    FreqMeter.h
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------*/
 
#ifndef __FREQMETER_H
#define __FREQMETER_H

#include "STM32F4xx.h"

extern __IO uint16_t DutyCycle;
extern __IO uint16_t Frequency;
extern __IO uint16_t IC2Value;

extern void TIM_Config(void);
extern void Freq_Meter_Init(void);
extern void TIM4_IRQHandler(void);

#endif
