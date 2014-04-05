/*----------------------------------------------------------------------------
 * Name:    DDS.c
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/
  
 #include "STM32F4xx.h"
 #include "main.h"
 #include "DDS.h"
 #include <math.h>
 
 void DDS_write(int pin, int state)
 { 
	 if ( state == LOW) 
	 {
			GPIOE->BSRRL |= ( 1UL << pin );
	 }
	 else if (state == HIGH) 
	 {
		  GPIOE->BSRRH |= ( 1UL << pin );
	 }
 }

void DDS_init()
{
	RCC->AHB1ENR    |=  ((1UL <<  4)    );   /* Enable GPIOE clock              */
	
	GPIOE->MODER    &= ~((3UL << 2* 3) |
                       (3UL << 2* 4) |
                       (3UL << 2* 5) |
                       (3UL << 2* 6) );		/* PE.3..6 is output								*/                     
	
	GPIOE->MODER		|=	((1UL << 2* 3) |
                       (1UL << 2* 4) |
                       (1UL << 2* 5) |
                       (1UL << 2* 6) );		/* PE.3..6 is output								*/

	GPIOE->OTYPER		&= ~((1UL << 3) |
                       (1UL << 4) |
                       (1UL << 5) |
                       (1UL << 6) );		/* PE.3..6 is push-pull for output		*/	
	
  GPIOE->OSPEEDR  |=  ((3UL << 2* 3) |		
                       (3UL << 2* 4) | 
                       (3UL << 2* 5) | 
                       (2UL << 2* 6) ); /* PE.3..6 is High Speed          */
	
  GPIOE->PUPDR    &= ~((3UL << 2* 3) |
                       (3UL << 2* 4) |
                       (3UL << 2* 5) |
                       (3UL << 2* 6) );
											 
	GPIOE->PUPDR		|=  ((2UL << 2* 3) |
											 (2UL << 2* 4) |
											 (2UL << 2* 5) |
											 (2UL << 2* 6));		/* PE.3..6 is pull down resistor	*/
											 
							
	
  DDS_write(RESET, LOW);
  DDS_write(CLOCK, LOW);
  DDS_write(LOAD, LOW);
  DDS_write(DATA, LOW);
}

void DDS_reset()
{
   //reset sequence is:
   // CLOCK & LOAD = LOW
   //  Pulse RESET high for a few uS (use 5 uS here)
   //  Pulse CLOCK high for a few uS (use 5 uS here)
   //  Set DATA to ZERO and pulse LOAD for a few uS (use 5 uS here)
  
   // data sheet diagrams show only RESET and CLOCK being used to reset the device, but I see no output unless I also
   // toggle the LOAD line here.
  
	 DDS_write(CLOCK, LOW);
	 DDS_write(LOAD, LOW);
    
	 DDS_write(RESET, LOW);
	 Delay(5);
	 DDS_write(RESET, HIGH);  //pulse RESET
	 Delay(5);
	 DDS_write(RESET, LOW);
	 Delay(5);
	 
	 DDS_write(CLOCK, LOW);
	 Delay(5);
	 DDS_write(CLOCK, HIGH);  //pulse CLOCK
	 Delay(5);
	 DDS_write(CLOCK, LOW);
	 Delay(5);
	 DDS_write(DATA, LOW);    //make sure DATA pin is LOW
	 
	 DDS_write(LOAD, LOW);
	 Delay(5);
	 DDS_write(LOAD, HIGH);  //pulse LOAD
	 Delay(5);
	 DDS_write(LOAD, LOW);
  // Chip is RESET now
}

void SetFrequency(unsigned long frequency)
{
	int i;
	
	// Calculate the tuning word and make sure its at least 32-bits long
  unsigned long tuning_word = (frequency * pow(2, 32)) / DDS_CLOCK;
	unsigned long tuning_word_32 = tuning_word | 0x00000000;
  
	// Take load low ready for new tuning word to be written
	DDS_write(LOAD, LOW); 
	
	// Write 32-bit tuning word
	for(i = 0 ; i < 32 ; i++)
	{
		int tuning_bit = GETBIT(i, tuning_word_32);
	
		DDS_write(DATA, tuning_bit);
	}
	
	// Write 2 CONTROL bits as 0
	DDS_write(DATA, 0);
	DDS_write(DATA, 0);
	
	// Write power-down bit as 0
	DDS_write(DATA, 0);
	
	// Write 5 phase bits as 0
	DDS_write(DATA, 0);
	DDS_write(DATA, 0);
	DDS_write(DATA, 0);
	DDS_write(DATA, 0);
	DDS_write(DATA, 0);
  
	// Take load high to set FQ_UP
  DDS_write(LOAD, HIGH); 
} 

