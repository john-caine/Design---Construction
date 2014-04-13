/*----------------------------------------------------------------------------
 * Name:    main.h
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------*/
 
#ifndef __MAIN_H
#define __MAIN_H

#define WAVE_GENERATION 0
#define FREQUENCY_METER 1
#define NOISE_GENERATION 2
#define ARBITORY_FUNCTION 3

extern void SysTick_Handler (void);
extern void Delay (uint32_t dlyTicks);
extern int main (void);
extern void Config_menu_interrupt(void);
extern void EXTI0_IRQHandler(void);

#endif
