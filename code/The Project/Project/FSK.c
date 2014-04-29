/*----------------------------------------------------------------------------
 * Name:    FSK.c
 * Purpose: 
 * Note(s): Example code taken from STMicroElectronics TIM_PWM_Input project
 *----------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/

#include "STM32F4xx.h"
#include "main_2.h"
#include "FSK.h"

volatile int FSK_Freq;
volatile int toggleBit;

void FSK_Init(void)
{  
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
  /* TIM Configuration */
  TIM2_Config();

  /* TIM2 configuration: PWM Input mode ------------------------
     The external signal is connected to TIM2 CH2 pin (PB.08), 
     The Rising edge is used as active edge,
     The TIM2 CCR2 is used to compute the frequency value 
     The TIM2 CCR1 is used to compute the duty cycle value
  ------------------------------------------------------------ */

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);

  /* Select the TIM2 Input Trigger: TI2FP2 */
  TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);

  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);

  /* TIM enable counter */
  TIM_Cmd(TIM2, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
}

void TIM2_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  /* TIM9 chennel2 configuration : PB.08 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Connect TIM pin to AF2 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM2);

  /* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

void TIM2_IRQHandler(void){
	
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  
	if (toggleBit == 1)
  {
		FSK_Freq = HIGH;
		toggleBit = 0;
  }
  else
  {
		FSK_Freq = LOW;
		toggleBit = 1;
  }
	
	/* Clear TIM2 Capture compare interrupt pending bit */
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
}
