/*----------------------------------------------------------------------------
 * Name:    FSK.h
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------*/
 
#ifndef __FSK_H
#define __FSK_H

#define THRESHOLD 1
#define HIGH 1
#define LOW 0

#include "STM32F4xx.h"

extern volatile int FSK_Freq;
extern volatile uint16_t IC2Value;

extern void TIM2_Config(void);
extern void FSK_Init(void);
extern void TIM2_IRQHandler(void);

#endif
