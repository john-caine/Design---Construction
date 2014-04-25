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

#define TIM3_CLK_OUT 42000
#define TIM3_CNT_CLK 28000000
#define TIM3_ARR 665 //((TIM3_CNT_CLK / TIM3_CLK_OUT) - 1)
#define FIFTY_PERCENT 333

void Pulse_Config (void) {
	
	TIM3_Config();
	PWM_Config(TIM3_ARR);
}

void TIM3_Config (void) {
	GPIO_InitTypeDef GPIO_InitStructure;
 
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 
  /* GPIOC clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
   
  /* GPIOC Configuration: TIM3 CH1 (PC6) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	/* Connect TIM3 pins to AF2 */ 
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
}

void PWM_Config(int period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
  uint16_t PrescalerValue = 0;
  
/* Compute the prescaler value */
  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 28000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 665;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = FIFTY_PERCENT;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	/* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
}

void PWM_SetDC(uint16_t dutycycle)
{
	uint16_t newDutyCycle;
	
	newDutyCycle = (dutycycle * TIM3_ARR) / 100;
	
	TIM_SetCompare1(TIM3, newDutyCycle);
}
