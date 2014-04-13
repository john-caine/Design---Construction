/*----------------------------------------------------------------------------
 * Name:    ramp.c
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/

//SEE PAGE 436 in REF MANUAL - TRIANGLE WAVE GENERATEION WITH DAC

#include "STM32F4xx.h"
#include "main_2.h"
#include "ramp.h"

void DAC1_init() {
	DAC->CR |= ((11UL << 8)  |	// Set amplitude to 4095
							(1UL << 0)   |	// Enable DAC1 channel
						  (0UL << 1)	 |	// Enable output buffer
					    (1UL << 2)	 |	// Enable trigger events
					    (4UL << 3)	 |	// Set trigger events to Timer2
					    (2UL << 6)   |	// Set triangle wave generation 
				      (0UL << 12)  | 	// Disable DMA requests
					    (0UL << 13)); 	// Disable DMA underrun interrupt
	
}

