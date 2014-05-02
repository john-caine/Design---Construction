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
#include "DDS.h"

void FSK_Init(void)
{  
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
  /* TIM Configuration */
 // TIM4_FSK_Config();
	
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
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Connect TIM pin to AF2 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

  /* Enable the TIM4 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

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

void TIM4_FSK_Config(void)
{


}

//void TIM4_IRQHandler(void){
//  
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	FSK_Change = true;
//	
//	if (toggleBit == 1)
//  {
//		FSK_Freq = HIGH;
////		DDS_Set(100000);
//		toggleBit = 0;
//  }
//  else
//  {
//		FSK_Freq = LOW;
////		DDS_Set(800000);
//		toggleBit = 1;
//  }
//	
//	/* Clear TIM2 Capture compare interrupt pending bit */
//  TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

//}
