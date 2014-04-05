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
  
	if (SysTick_Config(SystemCoreClock / 1000)) {  /* SysTick 1 micro sec interrupts  */
    while (1);                                  /* Capture error              */
  }

  LED_Init();
	LED_On(1);
	Delay(1000);
	LED_Off(1);
	
//  BTN_Init();   
//  SWT_Init();
//	Square_Init();
//  LCD_Initpins();	
//	LCD_DriverOn();
//	
//	Delay(10000);
//	LCD_Init();

//	LCD_DriverOn();
//	LCD_On(1);
//	Delay(20000);
//	LCD_Clear();
//	LCD_PutS("Hello World!");
//	
//	LCD_GotoXY(4,1);
//	LCD_PutS("Testing Testing");
//	
//	Make_Square();

//	DDS_init();
//	Delay(10);
//	SetFrequency(1000);
//	
//	//while(1);
}

volatile uint32_t usTicks;                      /* counts 1us timeTicks       */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  usTicks++;
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

  curTicks = usTicks;
  while ((usTicks - curTicks) < dlyTicks);
}
