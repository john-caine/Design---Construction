/*----------------------------------------------------------------------------
 * Name:    DAC.h
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------*/
 
#ifndef __DAC_H
#define __DAC_H

/* LCD Definitions */

extern void DACs_Init(void);
extern void DAC_Ch2_TriangleConfig(void);
extern void DAC_Ch1_NoiseConfig(void);
extern void TIM6_Config(void);
extern void DAC_Triangle_On(void);
extern void DAC_Triangle_Off(void);
extern void DAC_Noise_On(void);
extern void DAC_Noise_Off(void);

#endif
