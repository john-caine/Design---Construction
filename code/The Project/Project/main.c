/**
  ******************************************************************************
  * @file    BoardTestProject/main.c 
  * @author  Ibrahim Ozturk -- www.ozturkibrahim.com
  */ 
#include "inc/includes.h"

int main(void)
{
	//HSE_VALUE --> 8MHz on stm32f4xx_conf.h	
	SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }
		
	/*excelde olusturalana gore : SysClk 32MHz, PresCaler AHBx : 2, Dolaysyla AHBx Freq = 16MHz oldu!*/
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f4xx.c file
  */

  //BTN_Init();   
  //SWT_Init();
	//Led_Init();
	
  //Led_Run();	
	//LCD_Run();
	//LCD_Clear();
	
	TIMER3_Config();
	
	while(1)
	{
	}
}

