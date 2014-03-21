/*----------------------------------------------------------------------------
 * Name:    DDS.c
 * Purpose: 
 * Note(s):
 *----------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/
 
#include "DDS.h"
#include "display.c"

void DDS_init()
{
  digitalWrite(RESET, LOW);
  digitalWrite(CLOCK, LOW);
  digitalWrite(LOAD, LOW);
  digitalWrite(DATA, LOW);
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
  
	 digitalWrite(CLOCK, LOW);
	 digitalWrite(LOAD, LOW);
    
	 digitalWrite(RESET, LOW);
	 Delay(5);
	 digitalWrite(RESET, HIGH);  //pulse RESET
	 Delay(5);
	 digitalWrite(RESET, LOW);
	 Delay(5);
	 
	 digitalWrite(CLOCK, LOW);
	 Delay(5);
	 digitalWrite(CLOCK, HIGH);  //pulse CLOCK
	 Delay(5);
	 digitalWrite(CLOCK, LOW);
	 Delay(5);
	 digitalWrite(DATA, LOW);    //make sure DATA pin is LOW
	 
	 digitalWrite(LOAD, LOW);
	 Delay(5);
	 digitalWrite(LOAD, HIGH);  //pulse LOAD
	 Delay(5);
	 digitalWrite(LOAD, LOW);
  // Chip is RESET now
}

void SetFrequency(unsigned long frequency)
{
  unsigned long tuning_word = (frequency * pow(2, 32)) / DDS_CLOCK;
  digitalWrite (LOAD, LOW); 

  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word);
  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word >> 8);
  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word >> 16);
  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word >> 24);
  shiftOut(DATA, CLOCK, LSBFIRST, 0x0);
  
  digitalWrite (LOAD, HIGH); 
}

