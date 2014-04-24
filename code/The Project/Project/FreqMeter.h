/*----------------------------------------------------------------------------
 * Name:    FreqMeter.h
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------*/
 
#ifndef __FREQMETER_H
#define __FREQMETER_H

#include "STM32F4xx.h"

#define LESS_THAN_1 0
#define ONE_TO_100 1
#define HUNDRED_TO_10K 2
#define MORE_THAN_10K 3

extern volatile int freqRange;
extern volatile uint16_t DutyCycle;
extern volatile uint32_t Frequency;
extern volatile double low_Frequency;
extern volatile uint16_t IC2Value;

extern void TIM_Config(void);
extern void Freq_Meter_Init(void);
extern void TIM4_IRQHandler(void);

#endif
