/*----------------------------------------------------------------------------
 * Name:    DDS.h
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------*/
 
#ifndef __DDS_H
#define __DDS_H

 #define DDS_CLOCK 125000000
 #define RESET 5 /* RST pin */
 #define CLOCK 6 /* W_CLK pin */
 #define LOAD	 4 /* FQ_UP pin*/
 #define DATA	 3 /* DATA pin */
 
 #define LOW 0
 #define HIGH 1	
 
 #define GETBIT(bit, word) ((word >> bit) & 1);

extern void DDS_init(void);
extern void DDS_reset(void);
extern void SetFrequency(unsigned long frequency);
extern void DDS_write(int pin, int state);

#endif
