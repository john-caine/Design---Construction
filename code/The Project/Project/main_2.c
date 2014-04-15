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
	
	//Initialise components to defaults
	DDS_Default_Init();	
	DAC_Ch2_TriangleConfig();
	DAC_Ch1_NoiseConfig();
	
	LCD_GotoXY(0,0);
	Delay(1);
	LCD_PutS("Testing");
	
	// Set up intterupts for the blue user button - ie the menu
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);
	//Config_menu_interrupt();
	
	DDS_Set(5000);
	
	
	while(function == WAVE_GENERATION) {
		
		uint32_t switchsState;
		
		LCD_Clear();
		LCD_GotoXY(0, 0);
		LCD_PutS("SQUARE&SIN WAVES");
		
		// Check for switch presses to change DDS fequency
		switchsState = SWT_Get();
		
		if (switchsState == (1UL << 8)) {
			increment = 0.01;
			LCD_Clear();
			LCD_GotoXY(0, 1);
			LCD_PutS("Inc = 0.01");
		}
		else if (switchsState == (1UL << 9)) {
			increment = 1;
			LCD_Clear();
			LCD_GotoXY(0, 1);
			LCD_PutS("Inc = 1");
		}
		else if (switchsState == (1UL << 10)) {
			increment = 100;
			LCD_Clear();
			LCD_GotoXY(0, 1);
			LCD_PutS("Inc = 100");
		}
		else if (switchsState == (1UL << 11)) {
			increment = 1000;
			LCD_Clear();
			LCD_GotoXY(0, 1);
			LCD_PutS("Inc = 1000");
		}
		else if (switchsState == (1UL << 12)) {
			increment = 100000;
			LCD_Clear();
			LCD_GotoXY(0, 1);
			LCD_PutS("Inc = 10000");
		}
		else if (switchsState == (1UL << 13)) {
			increment = 1000000;
			LCD_Clear();
			LCD_GotoXY(0, 1);
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
	
	while(function == FREQUENCY_METER) {
		
		uint32_t switchsState;
		
//		LCD_Clear();
//		LCD_GotoXY(0, 0);
//		LCD_PutS("FREQUENCY METER");
		
		// Check for switch presses to chnage DDS fequency
		switchsState = SWT_Get();
		
		if (switchsState == (1UL << 15)) {
			char Freq_Tmp[15];
			char DC_Tmp[15];
			
			Freq_Meter_Init();
			
			LCD_Clear();
			Delay(1);
			LCD_GotoXY(0, 0);
			sprintf(Freq_Tmp, "Freq = %d", Frequency);
			LCD_PutS(Freq_Tmp);
			Delay(1);
			LCD_GotoXY(0, 1);
			sprintf(DC_Tmp, "D/C = %d", DutyCycle);
			LCD_PutS(DC_Tmp);
		}
	}
	
	while(function == TRIANGLE_GENERATION) {
		LCD_Clear();
		LCD_GotoXY(0, 0);
		LCD_PutS("TRIANGLE WAVE");
		
		//TODO - change frequency using switches
	}
	
	while(function == NOISE_GENERATION) {
		LCD_Clear();
		LCD_GotoXY(0, 0);
		LCD_PutS("NOISE GENERATION");
	}
	
	while(function == ARBITORY_FUNCTION) {
		LCD_Clear();
		LCD_GotoXY(0, 0);
		LCD_PutS("ARBITORY FUNC");
	}
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
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; // chnaged from 0x01
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;				// chnaged from 0x01
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
}

 void EXTI0_IRQHandler(void) {
	 
	if (function == WAVE_GENERATION) 
		{
			function = FREQUENCY_METER;
			
//			LCD_Clear();
			LCD_GotoXY(0, 0);
			LCD_PutS("FREQUENCY METER");
		}
	else if (function == FREQUENCY_METER)
		{
			function = NOISE_GENERATION;
		}
	else if (function == NOISE_GENERATION)
		{
			function = ARBITORY_FUNCTION;
		}
	else if (function == ARBITORY_FUNCTION)
		{
			function = WAVE_GENERATION;
		}
		
	EXTI_ClearITPendingBit(EXTI_Line0);				// Clear the pending bit to signal IRQ finished
}
