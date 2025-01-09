#include "BUTTONS.h"

void initButtonsAndButtonInterrupts(void)
{
    pinMode(2, INPUT_PULLUP); // Asetetaan pinnit INPUT_PULLUP-tilaan
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);


    PCICR |= (1 << PCIE2); //Kesksytykset PORTD-pinnien muutoksille
    PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22);
    // Pinnit, joille halutaan keskeytys asetetaan PCMSK2-rekisteriin
}
