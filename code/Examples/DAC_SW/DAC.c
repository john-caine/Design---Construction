/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: COPIES SWITCH VALUES TO LEDS ON CARRIER BOARD
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/
 
 /* MODIFIED BY D. CHESMORE JANUARY 2013 */
 
#include <stdio.h>
#include "STM32F4xx.h"
#include "LED.h"
#include "SWT.h"
#include "LCD.h"



volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {                                              
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

/* Function to intiialise ADC1    */

void DAC_init(void) {
	
	RCC->AHB1ENR  |= ((1UL <<  0) );         /* Enable GPIOA clock                */
	RCC->APB1ENR  |= ((1UL <<  29) );         /* Enable DAC clock                */
	GPIOA->MODER |= ((3UL << 2*4) | (3UL <<5));		/* set pin 4  and 5 to analouge */
	GPIOA->PUPDR &= (~(3UL <<2*4) | ~(3UL <<2*5));
  DAC->CR |= (1UL << 16) | (1UL << 0);					/* turn DAC 1 on (bit 0) and DAC 2 on (bit 16)*/
	
}
	
/* function to write DAC1 */
void write_DAC1 (uint32_t val) {
	
	DAC->DHR12RD = val;
	return;
}

/* function to write DAC2 */
void write_DAC2 (uint32_t val) {
	
	DAC->DHR12LD = val;
	return;
}

	
/*----------------------------------------------------------------------------
  Function that initializes Button pins
 *----------------------------------------------------------------------------*/
void BTN_Init(void) {

  RCC->AHB1ENR  |= ((1UL <<  0) );              /* Enable GPIOA clock         */

  GPIOA->MODER    &= ~((3UL << 2*0)  );         /* PA.0 is input              */
  GPIOA->OSPEEDR  &= ~((3UL << 2*0)  );         /* PA.0 is 50MHz Fast Speed   */
  GPIOA->OSPEEDR  |=  ((2UL << 2*0)  ); 
  GPIOA->PUPDR    &= ~((3UL << 2*0)  );         /* PA.0 is no Pull up         */

	

}

/*----------------------------------------------------------------------------
  Function that read User Button pin
 *----------------------------------------------------------------------------*/
 uint32_t BTN_Get(void) {

 return (GPIOA->IDR & (1UL<<0));
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/

int main (void) {

 uint32_t btns = 0;


  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }

  LED_Init();
  BTN_Init();   
  SWT_Init();	
	DAC_init();

	GPIOD->ODR = 0;																/* turn LEDs off */
  while(1) {                                    /* Loop forever               */
    btns = SWT_Get();                           /* Read switch states         */
		write_DAC1(btns);
		write_DAC2(btns);
		
		Delay(50);
	}

	
				
}

