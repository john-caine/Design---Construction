/*----------------------------------------------------------------------------
 * Name:    DDS.c
 * Purpose: Fucntions to initialise the DDS, set default data, and accept new
						frequencies from the user.
 * Note(s):
 *----------------------------------------------------------------------------*/

#include "STM32F4xx.h"
#include "DDS.h"
#include "main_2.h"
#include <math.h>

#define DDS_CLOCK 125000000
#define CLOCK 4 /* W_CLK pin */
#define LOAD	 5 /* FQ_UP pin*/
#define DATA	 3 /* DATA pin */

/*----------------------------------------------------------------------------
  initialize DDS for serial communication
 *----------------------------------------------------------------------------*/
void DDS_Init (void) {

  RCC->AHB1ENR    |= ((1UL <<  4));         /* Enable GPIOE clock */

  GPIOE->MODER    &= ~((3UL << 2* 3) |       
                       (3UL << 2* 4) |
                       (3UL << 2* 5) );   /* PE.0,3-4 are outputs */
  GPIOE->MODER    |=  ((1UL << 2* 3) |
                       (1UL << 2* 4) |
                       (1UL << 2* 5) );
  GPIOE->OTYPER   &= ~((1UL <<    3) |
                       (1UL <<    4) |
                       (1UL <<    5) );   /* PE.0,3-4 are output Push-Pull */
  GPIOE->OSPEEDR  &= ~((3UL << 2* 3) |
                       (3UL << 2* 4) |
                       (3UL << 2* 5) );   /* PE.0,3-4 are 50MHz Fast Speed */
  GPIOE->OSPEEDR  |=  ((2UL << 2* 3) |
                       (2UL << 2* 4) |
                       (2UL << 2* 5) ); 
  GPIOE->PUPDR    &= ~((3UL << 2* 3) |
                       (3UL << 2* 4) |
                       (3UL << 2* 5) );   /* PE.0,3-4 are Pull up */
  GPIOE->PUPDR    |=  ((1UL << 2* 3) |
                       (1UL << 2* 4) |
                       (1UL << 2* 5) ); 
}

void Pulse_Clock() {
	GPIOE->ODR |= (1 << CLOCK);
	Delay(1);
	GPIOE->ODR &= ~(1 << CLOCK);
}

void Pulse_Frequency() {
	GPIOE->ODR |= (1 << LOAD);
	Delay(1);
	GPIOE->ODR &= ~(1 << LOAD);
}

void Data_Low() {
	GPIOE->ODR &= ~(1 << DATA);
}

void DDS_Write_Data(int input_data) {
	GPIOE->ODR |= (input_data << DATA);
}

/*----------------------------------------------------------------------------
  Function that set the DDS output to a default value
 *----------------------------------------------------------------------------*/
void DDS_Default_Init (void) {
	
	int i = 0;
	int Default_Data[40] = {0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //1KHz

	Pulse_Clock();
	Delay(1);
	Pulse_Frequency();
	Delay(1);
	
	// Send the data array 1 bit at a time to the DDS
	for(i = 0; i <40; i++){
		Data_Low();
		DDS_Write_Data(Default_Data[i]);
		Delay(1);
		Pulse_Clock();
	}
	
	Pulse_Frequency();
	Delay(1);
	Data_Low();

}
/*----------------------------------------------------------------------------
  Function that sets the DDS frequency to a user provided value
 *----------------------------------------------------------------------------*/
void DDS_Set (double frequency) {
	
	int j = 0;
	int k = 0;
	int tuningWord = 0;
	int Send_Data[40];
	
	// Calculate the new tuning word
	tuningWord = (int) ((frequency * pow(2, 32))/DDS_CLOCK);
	
	// Construct the data array ready to be sent to DDS
	for (j = 0; j < 40; j++) {
		// calculate each array position by bitwise anding the tuning word with 1
		Send_Data[j] = tuningWord & (1 << j) ? 1 : 0;
	}
	
	Pulse_Clock();
	Delay(1);
	Pulse_Frequency();
	Delay(1);
	
	// Send the data array 1 bit at a time to the DDS
	for(k = 0; k <40; k++){
		Data_Low();
		DDS_Write_Data(Send_Data[k]);
		Delay(1);
		Pulse_Clock();
	}
	
	Pulse_Frequency();
	Delay(1);
	Data_Low();
}
