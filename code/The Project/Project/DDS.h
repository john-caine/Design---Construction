/*----------------------------------------------------------------------------
 * Name:    DDS.h
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------*/
 
#ifndef __DDS_H
#define __DDS_H

 #define DDS_CLOCK 125000000
 #define CLOCK 4 /* W_CLK pin */
 #define LOAD	 5 /* FQ_UP pin*/
 #define DATA	 3 /* DATA pin */
 
// #define GETBIT(bit, word) ((word >> bit) & 1);

extern void DDS_Init(void);
extern void DDS_Default_Init(void);
extern void DDS_Set(double);
extern void Pulse_Clock(void);
extern void Pulse_Frequency(void);
extern void Data_Low(void);

#endif
