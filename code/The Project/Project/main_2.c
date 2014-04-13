/*----------------------------------------------------------------------------
 * Name:    main.c
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/

#include "STM32F4xx.h"
#include "stm32f4_discovery.h"
#include "main_2.h"
#include "LED.h"
#include "SWT.h"
#include "LCD.h"
#include "Sqaure.h"
#include "DAC.h"
#include "DDS.h"
#include <stdio.h>

double currentFrequency = 1000;
double increment = 1;
int function = WAVE_GENERATION;

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  
	if (SysTick_Config(SystemCoreClock / 1000)) {  /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }
	
	// Enable interrupts for the device
	__enable_irq();
	
  // Initialise Required Pins
	BTN_Init();   
  SWTS_Init();
  LCD_Initpins();
	DDS_Init();
	DACs_Init();
	
	// Turn the LCD on
	LCD_DriverOn();
	Delay(10);
	LCD_Init();
	LCD_DriverOn();
	LCD_On(1);
	Delay(2);
	LCD_Clear();
	
	//Initialise the DDS for sine & square waves
	DDS_Default_Init();
	
	// Initialise the DACs for Triangle & Noise generation
	DAC_Ch2_TriangleConfig();
	DAC_Ch1_NoiseConfig();
	
	LCD_GotoXY(0,0);
	LCD_PutS("Testing");
	
	// Set up intterupts for the blue user button - ie the menu
	//STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);
	//Config_menu_interrupt();
	
	//EXTI_GenerateSWInterrupt(EXTI_Line0);
	
	// Check for switch presses to chnage DDS fequency
	while(1) {
		uint32_t switchsState = SWT_Get();
		
		if (switchsState == (1UL << 8)) {
			increment = 0.01;
			LCD_Clear();
			LCD_GotoXY(0, 0);
			LCD_PutS("Inc = 0.01");
		}
		else if (switchsState == (1UL << 9)) {
			increment = 1;
			LCD_Clear();
			LCD_GotoXY(0, 0);
			LCD_PutS("Inc = 1");
		}
		else if (switchsState == (1UL << 10)) {
			increment = 100;
			LCD_Clear();
			LCD_GotoXY(0, 0);
			LCD_PutS("Inc = 100");
		}
		else if (switchsState == (1UL << 11)) {
			increment = 1000;
			LCD_Clear();
			LCD_GotoXY(0, 0);
			LCD_PutS("Inc = 1000");
		}
		else if (switchsState == (1UL << 12)) {
			increment = 100000;
			LCD_Clear();
			LCD_GotoXY(0, 0);
			LCD_PutS("Inc = 10000");
		}
		else if (switchsState == (1UL << 13)) {
			increment = 1000000;
			LCD_Clear();
			LCD_GotoXY(0, 0);
			LCD_PutS("Inc = 1000000");
		}
		else if (switchsState == (1UL << 14)) {
			currentFrequency = currentFrequency - increment;
			DDS_Set(currentFrequency);
			LCD_GotoXY(0, 1);
			LCD_PutS("Freq = ");
		}
		else if (switchsState == (1UL << 15)) {
			currentFrequency = currentFrequency + increment;
			DDS_Set(currentFrequency);
			LCD_GotoXY(0, 1);
			LCD_PutS("Freq = ");
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

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

void Config_menu_interrupt(void) {
    EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	  /* Connect EXTI Line0 to GPIOA Pin 0*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

    /* Configure EXTI line0 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI Line0 Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
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
		
	EXTI_ClearITPendingBit(EXTI_Line0);				// Clear the pending bit to signal IRQ finished
}
