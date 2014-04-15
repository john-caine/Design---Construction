/*----------------------------------------------------------------------------
 * Name:    DAC.c
 * Purpose: 
 * Note(s): Example code taken from STMicroElectronics DAC_SignalsGeneration project
 *----------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/

#include "STM32F4xx.h"
#include "stm32f4_discovery.h"
#include <stdio.h>
#include "main_2.h"
#include "LCD.h"
#include "FreqMeter.h"

TIM_ICInitTypeDef  TIM_ICInitStructure;
__IO uint16_t DutyCycle;
__IO uint16_t Frequency;
__IO uint16_t IC2Value;

void Freq_Meter_Init(void)
{      
  /* TIM Configuration */
  TIM_Config();

  /* TIM4 configuration: PWM Input mode ------------------------
     The external signal is connected to TIM4 CH2 pin (PB.07), 
     The Rising edge is used as active edge,
     The TIM4 CCR2 is used to compute the frequency value 
     The TIM4 CCR1 is used to compute the duty cycle value
  ------------------------------------------------------------ */

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_PWMIConfig(TIM4, &TIM_ICInitStructure);

  /* Select the TIM4 Input Trigger: TI2FP2 */
  TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2);

  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(TIM4,TIM_MasterSlaveMode_Enable);

  /* TIM enable counter */
  TIM_Cmd(TIM4, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
}

void TIM_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  /* TIM4 chennel2 configuration : PB.07 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Connect TIM pin to AF2 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

  /* Enable the TIM4 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

void TIM4_IRQHandler(void){
	
//	char Freq_Tmp[15];
//	char DC_Tmp[15];
	
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  
	/* Get the Input Capture value */
  IC2Value = TIM_GetCapture2(TIM4);
  
	if (IC2Value != 0)
  {
		/* Duty cycle computation */
		DutyCycle = (TIM_GetCapture1(TIM4) * 100) / IC2Value;
		
		/* Frequency computation TIM4 counter clock = (RCC_Clocks.HCLK_Frequency)/2 */
		Frequency = ((RCC_Clocks.HCLK_Frequency)/2 / IC2Value);
  }
  else
  {
		DutyCycle = 0;
		Frequency = 0;
  }
	
	/* Clear TIM4 Capture compare interrupt pending bit */
  TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
}
