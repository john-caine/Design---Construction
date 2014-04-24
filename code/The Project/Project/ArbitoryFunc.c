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
#define   WAVE_RES          128                                   // Waveform resolution
#define   CNT_FREQ          84000000                             // TIM6 counter clock (prescaled APB1)
#define   TIM_PERIOD        ((CNT_FREQ)/((WAVE_RES)*(OUT_FREQ))) // Autoreload reg value

const uint16_t waveForm[WAVE_RES] = { 22842,	55745,	10,			63004,	9065,		23173,	48081,	31715,	64836,	59978,
																			43514,	29550,	53862,	30236,	44799,	45137,	61201,	10700,	44135,	28356,
																			50154,	62040,	63602,	47828,	2380,		1079,		29948,	41630,	23411,	29405,
																			46651,	54394,	22265,	3175,		41880,	63622,	62290,	13943,	32622,	22887,
																			3487,		20780,	59403,	53559,	47243,	1039,		64845,	63788,	30769,	62197,
																			29014,	25459,	15397,	40340,	8874,		43869,	52651,	36465,	57119,	19380,
																			62304,	18846,	55853,	17121,	1816,		56667,	30870,	32362,	2893,		36914,
																			40753,	33544,	18528,	38641,	30828,	52664,	22308,	50938,	17338,	14442,
																			3912,		28193,	6462,		35390,	48104,	25356,	35111,	12130,	31807,	26551,
																			31887,	46907,	56875,	38690,	37965,	51474,	22537,	50651,	41614,	38955,
																			63083,	65002,	28862,	12050,	21682,	12919,	32779,	13776,	63366,	20572,
																			21164,	47577,	65452,	40726,	61685,	617,		24727,	5885,		59062,	41856,
																			13418,	60594,	38817,	18505,	17037,	47952,	11696,	11495};            

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
  DMA_INIT.DMA_Memory0BaseAddr    = (uint32_t)&waveForm;
  DMA_INIT.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
  DMA_INIT.DMA_BufferSize         = WAVE_RES;
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
