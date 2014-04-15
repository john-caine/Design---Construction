/*----------------------------------------------------------------------------
 * Name:    main.h
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------*/
 
#ifndef __MAIN_H
#define __MAIN_H

#define WAVE_GENERATION 0
#define TRIANGLE_GENERATION 1
#define FREQUENCY_METER 2
#define NOISE_GENERATION 3
#define ARBITORY_FUNCTION 4

extern void SysTick_Handler (void);
extern void Delay (uint32_t dlyTicks);
extern int main (void);
extern void Config_menu_interrupt(void);
extern void EXTI0_IRQHandler(void);

#endif
