/*----------------------------------------------------------------------------
 * Name:    ArbitoryFunc.c
 * Purpose: 
 * Note(s): adapted from example code found at 
 *						http://00xnor.blogspot.co.uk/2014/01/6-stm32-f4-dac-dma-waveform-generator.html
 *----------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/

#include "STM32F4xx.h"
#include "main_2.h"

#define   DAC_DHR12R1_ADDR  0x40007408  
#define   OUT_FREQ          5000                                 // Output waveform frequency
#define   SINE_RES          32                                   // Waveform resolution
#define   CNT_FREQ          84000000                             // TIM6 counter clock (prescaled APB1)
#define   TIM_PERIOD        ((CNT_FREQ)/((SINE_RES)*(OUT_FREQ))) // Autoreload reg value

const uint16_t function[SINE_RES] = { 6, 827, 38, 2339, 264, 1, 2624, 99, 592, 999, 
                                      2984, 62, 625, 3230, 307, 31, 3451, 627, 1581, 3640, 
                                      55, 1748, 4019, 457, 77, 3911, 941, 396, 2647, 4002, 
                                      2, 666 };            

void TIM5_Config(void)
{
  TIM_TimeBaseInitTypeDef TIM5_TimeBase;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
 
  TIM_TimeBaseStructInit(&TIM5_TimeBase); 
  TIM5_TimeBase.TIM_Period        = (uint16_t)TIM_PERIOD;          
  TIM5_TimeBase.TIM_Prescaler     = 0;       
  TIM5_TimeBase.TIM_ClockDivision = 0;    
  TIM5_TimeBase.TIM_CounterMode   = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM6, &TIM5_TimeBase);
  TIM_SelectOutputTrigger(TIM5, TIM_TRGOSource_Update);

  TIM_Cmd(TIM5, ENABLE);
}

void DAC_Ch1_ArbitoryConfig(void)
{
  DAC_InitTypeDef DAC_INIT;
  DMA_InitTypeDef DMA_INIT;
  
  DAC_INIT.DAC_Trigger        = DAC_Trigger_T5_TRGO;
  DAC_INIT.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_INIT.DAC_OutputBuffer   = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_INIT);

  DMA_DeInit(DMA1_Stream5);
  DMA_INIT.DMA_Channel            = DMA_Channel_7;  
  DMA_INIT.DMA_PeripheralBaseAddr = (uint32_t)DAC_DHR12R1_ADDR;
  DMA_INIT.DMA_Memory0BaseAddr    = (uint32_t)&function;
  DMA_INIT.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
  DMA_INIT.DMA_BufferSize         = SINE_RES;
  DMA_INIT.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_INIT.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_INIT.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_INIT.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  DMA_INIT.DMA_Mode               = DMA_Mode_Circular;
  DMA_INIT.DMA_Priority           = DMA_Priority_High;
  DMA_INIT.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
  DMA_INIT.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_INIT.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_INIT.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream5, &DMA_INIT);

  DMA_Cmd(DMA1_Stream5, ENABLE);
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}

void DAC_Arbitory_On(void)
{
	/* Enable DAC Channel1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
}

void DAC_Arbitory_Off(void)
{
	/* Disable DAC Channel1 */
  DAC_Cmd(DAC_Channel_1, DISABLE);
}
