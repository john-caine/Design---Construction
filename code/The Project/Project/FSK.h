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
#include "stdbool.h"

extern volatile bool FSK_Change;
extern volatile int FSK_Freq;
extern volatile int toggleBit;

extern void TIM4_FSK_Config(void);
extern void FSK_Init(void);
extern void TIM4_IRQHandler(void);

#endif
