/*----------------------------------------------------------------------------
 * Name:    DDS.c
 * Purpose: DDS functions
 * Note(s):
 *----------------------------------------------------------------------------*/

#include "STM32F4xx.h"
#include "DDS.h"
#include "main_2.h"
#include <math.h>

/*----------------------------------------------------------------------------
  initialize DDS for serial communication
 *----------------------------------------------------------------------------*/
void DDS_Init (void) {

  RCC->AHB1ENR    |= ((1UL <<  4));         /* Enable GPIOE clock                */

  GPIOE->MODER    &= ~((3UL << 2* 3) |       
                       (3UL << 2* 4) |
                       (3UL << 2* 5) );   /* PE.0,3-4 are outputs               */
  GPIOE->MODER    |=  ((1UL << 2* 3) |
                       (1UL << 2* 4) |
                       (1UL << 2* 5) );
  GPIOE->OTYPER   &= ~((1UL <<    3) |
                       (1UL <<    4) |
                       (1UL <<    5) );   /* PE.0,3-4 are output Push-Pull     */
  GPIOE->OSPEEDR  &= ~((3UL << 2* 3) |
                       (3UL << 2* 4) |
                       (3UL << 2* 5) );   /* PE.0,3-4 are 50MHz Fast Speed     */
  GPIOE->OSPEEDR  |=  ((2UL << 2* 3) |
                       (2UL << 2* 4) |
                       (2UL << 2* 5) ); 
  GPIOE->PUPDR    &= ~((3UL << 2* 3) |
                       (3UL << 2* 4) |
                       (3UL << 2* 5) );   /* PE.0,3-4 are Pull up              */
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
	//int Default_Data[40] = {0,1,1,0,0,0,1,1,1,0,1,1,0,1,1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //100KHz
	//int Default_Data[40] = {0,1,0,1,1,1,0,1,1,0,0,1,0,0,1,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //1MHz
	//int Default_Data[40] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //0.03Hz

	Pulse_Clock();
	Delay(1);
	Pulse_Frequency();
	Delay(1);
	
	for(i = 0; i <40; i++){
		Data_Low();
		DDS_Write_Data(Default_Data[i]);
		Delay(1);
		Pulse_Clock();
	}
	
	Pulse_Frequency();
	Data_Low();

}
/*----------------------------------------------------------------------------
  Function that sets the DDS frequency to a user provided value
 *----------------------------------------------------------------------------*/
void DDS_Set (double frequency) {
	
	int j = 0;
	int k = 0;
	int phase = 0;
	int Send_Data[40];
	
	phase = (int) ((frequency * pow(2, 32))/DDS_CLOCK);
	
	for (j = 0; j < 40; j++) {
			Send_Data[j] = phase & (1 << j) ? 1 : 0;
	}
	
	Pulse_Clock();
	Delay(1);
	Pulse_Frequency();
	Delay(1);
	
	for(k = 0; k <40; k++){
		Data_Low();
		DDS_Write_Data(Send_Data[k]);
		Delay(1);
		Pulse_Clock();
	}
	
	Pulse_Frequency();
	Data_Low();
}
