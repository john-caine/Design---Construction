/*----------------------------------------------------------------------------
 * Name:    main.c
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/

#include "STM32F4xx.h"
#include "main.h"
#include "LED.h"
#include "SWT.h"
#include "LCD.h"
#include "Sqaure.h"
#include "ramp.h"
#include "DDS.h"
#include <stdio.h>

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  
	if (SysTick_Config(SystemCoreClock / 1000)) {  /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }
	
  // Initialise Required Pins
	BTN_Init();   
  SWTS_Init();
  LCD_Initpins();
	DDS_Init();
	
	// Turn the LCD on
	LCD_DriverOn();
	Delay(10);
	LCD_Init();
	LCD_DriverOn();
	LCD_On(1);
	Delay(2);
	
	//Initialise the DDS for sine & square waves
	DDS_Default_Init();
	
	while(1);
}

volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 us)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {                                              
  uint32_t curTicks;
	
	// 1000us = 1ms
	// 10000us = 10ms
	// 100000us = 0.1s
	// 1000000us = 1s

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}
