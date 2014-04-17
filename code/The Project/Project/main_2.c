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
#include "FreqMeter.h"
#include "hd44780.h"
#include <stdio.h>

volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
__IO double currentFrequency = 1000;
__IO double increment = 1;
__IO int function = WAVE_GENERATION;
__IO unsigned char updateFlag = 1;

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
	LED_Init();
	
	while(1) {
		
		LED_On(0);
		Delay(1000);
		LED_Off(0);
		Delay(1000);
	}
}
	
//	init_lcd_driver();
//  hd44780_init(GPIOD, GPIOB, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_4,
//								GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7, HD44780_LINES_2, HD44780_FONT_5x8);
//	DDS_Init();
//	DACs_Init();
//	
//	
//	//Initialise components to defaults
//	DDS_Default_Init();	
//	DAC_Ch2_TriangleConfig();
//	DAC_Ch1_NoiseConfig();
//	
//	// Turn on LCD display
//	hd44780_display(true, false, false);
//	
//	// Set up intterupts for the blue user button - ie the menu
//	//STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);
//	Config_menu_interrupt();
//	
//	hd44780_print("first test");
//	
//	while(1) {}
//	
//	while(1) 
//	{	
//		if(function == WAVE_GENERATION) 
//		{
//			uint32_t switchsState;
//			
//			if(updateFlag == 1) 
//			{
//				updateFlag = 0;
//				hd44780_position(0, 0);
//				hd44780_print("WAVE GENERATION");
//			}
//			
//			// Check for switch presses to change DDS fequency
//			switchsState = SWT_Get();
//			
//			if (switchsState == (1UL << 8)) {
//				LED_All_Off();
//				LED_On(0);
//				increment = 0.01;
//				hd44780_clear();
//				hd44780_position(1, 0);
//				hd44780_print("Inc = 0.01    Hz");
//				updateFlag = 1;
//			}
//			else if (switchsState == (1UL << 9)) {
//				LED_All_Off();
//				LED_On(1);
//				increment = 1;
//				hd44780_clear();
//				hd44780_position(1, 0);
//				hd44780_print("Inc = 1       Hz");
//				updateFlag = 1;
//			}
//			else if (switchsState == (1UL << 10)) {
//				LED_All_Off();
//				LED_On(2);
//				increment = 100;
//				hd44780_clear();
//				hd44780_position(1, 0);
//				hd44780_print("Inc = 100     Hz");
//				updateFlag = 1;
//			}
//			else if (switchsState == (1UL << 11)) {
//				LED_All_Off();
//				LED_On(3);
//				increment = 1000;
//				hd44780_clear();
//				hd44780_position(1, 0);
//				hd44780_print("Inc = 1000    Hz");
//				updateFlag = 1;

//			}
//			else if (switchsState == (1UL << 12)) {
//				LED_All_Off();
//				LED_On(4);
//				increment = 100000;
//				hd44780_clear();
//				hd44780_position(1, 0);
//				hd44780_print("Inc = 10000   Hz");
//				updateFlag = 1;
//			}
//			else if (switchsState == (1UL << 13)) {
//				LED_All_Off();
//				LED_On(5);
//				increment = 1000000;
//				hd44780_clear();
//				hd44780_position(1, 0);
//				hd44780_print("Inc = 1000000 Hz");
//				updateFlag = 1;
//			}
//			else if (switchsState == (1UL << 14)) {
//				char tmp_string[15];
//				
//				LED_On(6);
//				
//				currentFrequency = currentFrequency - increment;
//				if(currentFrequency < 0.01)
//					currentFrequency = 0.01;
//				DDS_Set(currentFrequency);
//				
//				sprintf(tmp_string, "Freq = %.2f", currentFrequency);
//				hd44780_clear();
//				hd44780_position(1, 0);
//				hd44780_print(tmp_string);
//				updateFlag = 1;
//				
//				LED_Off(6);
//			}
//			else if (switchsState == (1UL << 15)) {
//				char tmp_string[15];
//				
//				LED_On(7);
//				
//				currentFrequency = currentFrequency + increment;
//				if(currentFrequency > 35000000)
//						currentFrequency = 35000000;
//				DDS_Set(currentFrequency);
//				
//				sprintf(tmp_string, "Freq = %.2f ", currentFrequency);
//				hd44780_clear();
//				hd44780_position(1, 0);
//				hd44780_print(tmp_string);
//				updateFlag = 1;
//				
//				LED_Off(7);
//			}
//		}
//		else if(function == FREQUENCY_METER)
//		{		
//			uint32_t switchsState;

//			if(updateFlag == 1) 
//			{
//				updateFlag = 0;
//				hd44780_position(0, 0);
//				hd44780_print("FREQUENCY METER");
//			}
//			
//			
//			// Check for switch presses to capture frequency meter value
//			switchsState = SWT_Get();
//			
//			if (switchsState == (1UL << 15)) {
//				char Freq_Tmp[15];
//				char DC_Tmp[15];
//				LED_On(7);
//				Freq_Meter_Init();
//				hd44780_clear();
//				hd44780_position(0, 0);
//				sprintf(Freq_Tmp, "Freq = %d", Frequency);
//				hd44780_print(Freq_Tmp);
//				hd44780_position(0, 14);
//				hd44780_print("Hz");
//				Delay(1);
//				hd44780_position(1, 0);
//				sprintf(DC_Tmp, "Duty = %d", DutyCycle);
//				hd44780_print(DC_Tmp);
//				LED_Off(7);
//			}
//		}
//		else if(function == NOISE_GENERATION)
//		{
//			if(updateFlag == 1) 
//			{
//				updateFlag = 0;
//				hd44780_position(0, 0);
//				hd44780_print("NOISE GENERATION");
//			}
//			
//			DAC_Noise_On();
//		}
//		else if(function == ARBITORY_FUNCTION)
//		{
//			if(updateFlag == 1) 
//			{
//				updateFlag = 0;
//				hd44780_position(0, 0);
//				hd44780_print("ARBITRARY FUNC");
//			}
//		}
//	}
//}

///*----------------------------------------------------------------------------
//  SysTick_Handler
// *----------------------------------------------------------------------------*/
//void SysTick_Handler(void) {
//  msTicks++;
//}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {                                              
	__IO uint32_t temp;
  __IO uint32_t InnerTemp;

	for(temp = 0 ; temp < dlyTicks; temp++)
	{
		InnerTemp = 0;
		
		for(InnerTemp = 0 ; InnerTemp == 2001; InnerTemp++)
		{
			__NOP;
		}		
	}
}

///*----------------------------------------------------------------------------
//  delays number of tick Systicks (happens every 1 ms)
// *----------------------------------------------------------------------------*/
//void Delay (uint32_t dlyTicks) {                                              
//  uint32_t curTicks;
//	curTicks = msTicks;
//	
//  while ((msTicks - curTicks) < dlyTicks);
//}

//void Config_menu_interrupt(void) {
//    EXTI_InitTypeDef EXTI_InitStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//	
//	  /* Connect EXTI Line0 to GPIOA Pin 0*/
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

//    /* Configure EXTI line0 */
//    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);

//    /* Enable and set EXTI Line0 Interrupt to the lowest priority */
//    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; // changed from 0x01
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;				// changed from 0x01
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure); 
//}

// void EXTI0_IRQHandler(void) {
//	 
//		hd44780_clear();
//		
//	 hd44780_print("Testing test");
//	 
//	 if (function == WAVE_GENERATION) 
//		{
//			updateFlag = 1;
//			function = FREQUENCY_METER;
//			LED_All_Off();
//			Delay(100);
//		}
//	else if (function == FREQUENCY_METER)
//		{
//			updateFlag = 1;
//			function = NOISE_GENERATION;
//			Delay(100);
//		}
//	else if (function == NOISE_GENERATION)
//		{
//			updateFlag = 1;
//			function = ARBITORY_FUNCTION;
//			DAC_Noise_Off();
//		}
//	else if (function == ARBITORY_FUNCTION)
//		{
//			updateFlag = 1;
//			function = WAVE_GENERATION;
//		}
//	else
//		{
//			updateFlag = 1;
//			function = WAVE_GENERATION;
//		}
//		
//	EXTI_ClearITPendingBit(EXTI_Line0);				// Clear the pending bit to signal IRQ finished
//}
