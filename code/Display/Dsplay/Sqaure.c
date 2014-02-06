/*----------------------------------------------------------------------------
 * Name:    Square.c
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/

#include "STM32F4xx.h"
#include "LCD.h"
#include "Sqaure.h"

void Square_Init (void){

  RCC->AHB1ENR    |=  ((1UL << 2)  );   					    	/* Enable GPIOC clock */
	GPIOC->MODER    = (1UL << 2* 0); 										  /* PC 0 is output */
	GPIOC->OTYPER   = (0UL << 0);												  /* PC 0 output is push-pull */
	GPIOC->OSPEEDR  = (3UL << 2* 0);											/* PC 0 is  high speed */
	GPIOC->PUPDR    = (0UL << 2* 0); 										  /* PC 0 is no pull up no pull down */
}

void Timer_Init (void) {
	
	TIM3_CR1 = 
	
	GPIOC->MODER    = (2UL << 2* 6); 										  /* PC 6 is Alternate Function mode */
}

void Make_Square (void) {
	
	while(1) {	
		GPIOC->ODR = 1;
		Delay(5); 
		GPIOC->ODR = 0;
		Delay(5);
	}
}
