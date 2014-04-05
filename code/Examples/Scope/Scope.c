/*----------------------------------------------------------------------------
 * Name:    Scope.c

reads ADC channel ADC1 IN 14 and outputs to DAC1

Uses ADC1 status register EOC flag to test end of conversion*/


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
  Function that read Button pins
 *----------------------------------------------------------------------------*/
uint32_t BTN_Get(void) {

 return (GPIOA->IDR & (1UL << 0));
}

/* Function to intiialise ADC1    */

void ADC1_init(void) {
	
	RCC->APB2ENR  |= ((1UL <<  8) );         /* Enable ADC1 clock                */
	RCC->AHB1ENR  |= ((1UL <<  2) );         /* Enable GPIOC clock                */
	GPIOC->MODER = 0xffffffff;
	GPIOC->PUPDR = 0;
	ADC1->CR1 = 0x00;
	ADC1->CR1 |= (1UL << 11);
	ADC1->CR2 = 0x00;
	ADC1->CR2 |= (1UL << 10) ;					/* right alignement of 12 bits */
	ADC->CCR = 0x00;
	ADC1->SQR1 = 0x01;								/* 1 conversion at a time */
	ADC1->SMPR1 = 0x00;
	ADC1->SMPR1 = 0x0300;
	ADC1->SQR1 = 0x01;
	ADC1->SQR3 = 0x0e;								/* ADC_IN14 = 0x0e: ADC_IN15 = 0x0f */
	ADC1->CR2 |= (1UL << 0);
}
	
/* function to read ADC and retun value */

unsigned int read_ADC1 (void) {

	ADC1->CR2 |= (1UL << 30);		/* set SWSTART to 1 to start conversion */
  while ((ADC1->SR && 2UL)==0) {
  }
	return (ADC1->DR);
}

/*  Initialise DACs   */

void DAC_Init(void) {
	
	RCC->AHB1ENR  |= ((1UL <<  0) );         /* Enable GPIOA clock                */
	RCC->APB1ENR  |= ((1UL <<  29) );         /* Enable DAC clock                */
	GPIOA->MODER |= ((3UL << 2*4) | (3UL <<5));		/* set pin 4  and 5 to analouge */
	GPIOA->PUPDR &= (~(3UL <<2*4) | ~(3UL <<2*5));
	DAC->CR |= (1UL << 16) | (1UL << 0);					/* turn DAC 1 on (bit 0) and DAC 2 on (bit 16)*/
}
	
/* function to write DAC1 */
void write_DAC1(uint32_t val) {
	
	DAC->DHR12R1 = val;
	return;
}

/* function to write DAC2 */

void write_DAC2(uint32_t val) {
	
	DAC->DHR12R2 = val;
	return;
}
/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
 
 uint32_t value = 0;
	float scale, result, factor;
  char buffer [16];
	int n;
	
  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }
 
	ADC1_init();
	LED_Init();
  BTN_Init();   
  SWT_Init();	
	DAC_Init();
  LCD_Initpins();	
	LCD_DriverOn();
	
	Delay(10);
	LCD_Init();

	LCD_DriverOn();
	LCD_On(1);
	Delay(20);
	LCD_Clear();
  
 scale = 3.3/4096;

  while(1) {                                    /* Loop forever               */
		value = read_ADC1();
		GPIOD->ODR = value*16;
		write_DAC1(value);
		result = value*scale;
		LCD_GotoXY(0,0);
		sprintf (buffer, "V=%6.3f Volts", result);
    LCD_PutS(buffer);
		Delay(100);
	}
}
		


