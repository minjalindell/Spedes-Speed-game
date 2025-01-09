#ifndef LEDS_H
#define LEDS_H
#include <arduino.h>

/*
  initializeLeds() subroutine intializes analog pins A2,A3,A4,A5
  to be used as outputs. Speden Spelit leds are connected to those
  pins.  
*/
void initializeLeds();

/*
  setLed(byte) sets correct led number given as 0,1,2 or 3
  led number 0 corresponds to led connected at Arduino pin A2
  led number 1 => Arduino pin A3
  led number 2 => Arduino pin A4
  led number 3 => Arduino pin A5
  
  parameters:
  byte ledNumber is 0,1,2 or 3
*/
void setLed(byte ledNumber);

/*
  clearAllLeds(void) subroutine clears all leds
*/
void clearAllLeds(void);

/*
  setAllLeds subroutine sets all leds
*/
void setAllLeds(void);
#endif