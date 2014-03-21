/*----------------------------------------------------------------------------
 * Name:    DDS.h
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------*/
 
#ifndef __DDS_H
#define __DDS_H

#define DDS_CLOCK 125000000
#define CLOCK 8  //pin connections for DDS
#define LOAD  9 
#define DATA  10
#define RESET 11

extern void DDS_init(void);
extern void DDS_reset(void);
extern void SetFrequency(unsigned long frequency);

#endif