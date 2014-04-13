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

double currentFrequency = 1000;
int function = WAVE_GENERATION;

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
	
	// Set up intterupts for the blue user button - ie the menu
	Config_menu_interrupt();
	
	// Check for switch presses to chnage DDS fequency
	while(1) {
		uint32_t switchsState = SWT_Get();
		
		if (switchsState == (1UL << 8)) {
			currentFrequency = currentFrequency + 0.01;
			DDS_Set(currentFrequency);
		}
		else if (switchsState == (1UL << 9)) {
			currentFrequency = currentFrequency + 1;
			DDS_Set(currentFrequency);
		}
		else if (switchsState == (1UL << 10)) {
			currentFrequency = currentFrequency + 100;
			DDS_Set(currentFrequency);
		}
		else if (switchsState == (1UL << 11)) {
			currentFrequency = currentFrequency + 1000;
			DDS_Set(currentFrequency);
		}
		else if (switchsState == (1UL << 12)) {
			currentFrequency = currentFrequency + 100000;
			DDS_Set(currentFrequency);
		}
		else if (switchsState == (1UL << 13)) {
			currentFrequency = currentFrequency + 1000000;
			DDS_Set(currentFrequency);
		}
		else if ((switchsState == (1UL << 15)) && (switchsState == (1UL << 13))) {
			currentFrequency = currentFrequency - 1000000;
			DDS_Set(currentFrequency);
		}
		else if ((switchsState == (1UL << 15)) && (switchsState == (1UL << 12))) {
			currentFrequency = currentFrequency - 100000;
			DDS_Set(currentFrequency);
		}
		else if ((switchsState == (1UL << 15)) && (switchsState == (1UL << 11))) {
			currentFrequency = currentFrequency - 1000;
			DDS_Set(currentFrequency);
		}
		else if ((switchsState == (1UL << 15)) && (switchsState == (1UL << 10))) {
			currentFrequency = currentFrequency - 100;
			DDS_Set(currentFrequency);
		}
		else if ((switchsState == (1UL << 15)) && (switchsState == (1UL << 9))) {
			currentFrequency = currentFrequency - 1;
			DDS_Set(currentFrequency);
		}
		else if ((switchsState == (1UL << 15)) && (switchsState == (1UL << 8))) {
			currentFrequency = currentFrequency - 0.1;
			DDS_Set(currentFrequency);
		}
	};
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

void Config_menu_interrupt(void) {
	EXTI->IMR |= (1UL << 0); 				// Mask EXTI 0 line
	EXTI->RTSR |= (1UL << 0);				// Set interrupt for rising edges
	SYSCFG->EXTICR[1] &= 0xFFFFFFF0;		// Set EXTI 0 to GPIOA 0
}

 void EXTI0_IRQHandler(void) {
	LCD_Clear();
	LCD_GotoXY(0, 0);
	 
	if (function == WAVE_GENERATION) 
		{
			function = FREQUENCY_METER;
			LCD_PutS("Frequency Meter");
			// Frequency_meter_init();
		}
	else if (function == FREQUENCY_METER)
		{
			function = NOISE_GENERATION;
			LCD_PutS("Noise Generation");
			// Generate_Noise();
		}
	else if (function == NOISE_GENERATION)
		{
			function = ARBITORY_FUNCTION;
			LCD_PutS("Arbitory Function");
			// Generate_arbitory_function();
		}
	else if (function == ARBITORY_FUNCTION)
		{
			function = WAVE_GENERATION;
			LCD_PutS("Wave Generation");
			DDS_Set(1000);
			//Triangle_frequency(1000);
		}
		
		EXTI->PR |= (1 << 0);					// Clear the pending bit to signal IRQ finished
}
