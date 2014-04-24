/*----------------------------------------------------------------------------
 * Name:    Square.h
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------*/
 
#ifndef __SQUARE_H
#define __SQUARE_H

#include "STM32F4xx.h"

extern volatile int dutyCycle;

extern void PWM_SetDC (uint16_t dutycycle);
extern void TIM3_Config (void);
extern void PWM_Config (int period);
extern void Pulse_Config(void);

#endif
