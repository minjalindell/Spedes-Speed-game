#ifndef BUTTONS_H
#define BUTTONS_H
#include <arduino.h>

extern volatile bool buttonPressed[5];
void setButtons();
void initButtonsAndButtonInterrupts(void);

#endif
