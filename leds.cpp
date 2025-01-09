#include "leds.h"

const int ledPins[] = {A2, A3, A4, A5}; // LEDien pinnit

void initializeLeds() {
  for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); ++i) { //Käy läpi kaikki pinnit, jotka määritelty taulukossa
    pinMode(ledPins[i], OUTPUT); //Asettaa määritellyt pinnit OUTPUT-tilaan
  }
}

void setLed(byte ledNumber) {
  clearAllLeds(); //Nollataan ledit ennen yksittäisen ledin sytytystä
  delay(150);
  if (ledNumber >= 0 && ledNumber < sizeof(ledPins) / sizeof(ledPins[0])) {  //Tarkistetaan, onhan annettu ledNumber sallitulla parametrialueella(0-3)
    digitalWrite(ledPins[ledNumber], HIGH); //Jos annettu ledNumber on sallitulla alueella, tämä ledNumber asetetaan tilaan HIGH(päälle)

  }
}

void clearAllLeds() {
  for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); ++i) { //Käy läpi jokaisen pinnin ledin
    digitalWrite(ledPins[i], LOW); //Asettaa pinnit tilaan LOW(sammuttaa ne)
  }
}

void setAllLeds() {
  for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); ++i) { //Käy läpi kaikki määritellyt pinnit
    digitalWrite(ledPins[i], HIGH); //Asetetaan ne tilaan HIGH(päälle)
  }
}
