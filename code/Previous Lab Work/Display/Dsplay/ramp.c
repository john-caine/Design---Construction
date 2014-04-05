/*----------------------------------------------------------------------------
 * Name:    ramp.c
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/

//SEE PAGE 436 in REF MANUAL - TRIANGLE WAVE GENERATEION WITH DAC

#include "STM32F4xx.h"
#include "main.h"
#include "ramp.h"

void DAC1_init() {
	DAC->CR |= ((1UL << 0)  |	// Enable DAC1 channel
						 (0UL << 1)	 |	// Enable output buffer
					   (1UL << 2)	 |	// Enable trigger events
					   (4UL << 3)	 |	// Set trigger events to Timer2
					   (2UL << 6)  |	// Set triangle wave generation
				     (11UL << 8) |	// Set amplitude to 4095
				     (0UL << 12) | 	// Disable DMA requests
					   (0UL << 13)); 	// Disable DMA underrun interrupt
	
}






//GPIOE->MODER    &= ~ ((3UL << 2* 7) );   /* PE.7 is input  									*/ 
// GPIOE->OSPEEDR  |=  ((2UL << 2* 7) ); 	/* PE.3..7 is High Speed          */
//  GPIOE->PUPDR    &= ~((3UL << 2* 7)  );   /* PE.7 is no Pull up or down    */




//void DAC1_Init (void) {
//	
//	DAC_CR = xx00000000000000xx00101100???001
//}

//void DAC2_Init (void) {
//	
//	DAC_CR = xx00101100???001xx00000000000000
//}

//void DAC1_Triangle_Init (void) {
//	
//	DAC_CR = xx00000000000000xx00101110???101
//}

//void Ramp_Init (void) {
//	
//	int count =
//	
//	while (1) {
//		
//	}
//	
//	DAC_DHR12R1						/* DAC1 12-bit right aligned data register */
//	DAC_DHR12R2
//}
