#include "display.h"
#include "arduino.h"

const int latchPin = 10; // Latch-pinni
const int clockPin = 11; // Clock-pinni
const int dataPin = 8; // Data-pinni
int buttonPressad = false;

// Näytön numerot digitaalisina segmentteinä (0-9)
uint8_t numerot[] ={
  0b10000000, // 0
  ~0b00001100, // 1
  ~0b10110111, // 2
  ~0b10011111, // 3
  ~0b11001100, // 4
  ~0b11011011, // 5
  ~0b11111010, // 6
  ~0b00001110, // 7
  0b00000000, // 8
  ~0b11011111, // 9
};

volatile int counter = 0; // Laskurin muuttuja


void initializeDisplay(){    // Asetetaan pinnit OUTPUT -tilaan
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  
}

void laskuri() { 
  if (buttonPressad) {
    counter++; // Lisää laskuria yhdellä
    if (counter > 99) {
      counter = 0; // Nollaa laskuri, jos se menee yli 99
    }
    buttonPressad = false; // Nollaa painallustunnistus
}
}

void naytaNumerot(){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, numerot[counter / 10]); // Toisen näytön numeron esitys
  shiftOut(dataPin, clockPin, MSBFIRST, numerot[counter % 10]); // Ensimmäisen näytön numeron esitys
  digitalWrite(latchPin, HIGH);
   delay(150);// Pieni viive näytöllä
}

void buttonISR() {
  buttonPressad = true; // Aseta painallustunnistus todeksi
}

void nolla(){ //Counterin nollaus
  counter = 0;
}


void nollaus(){ // näyttöjen nollaus pelin jälkeen
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, numerot[0]);
  shiftOut(dataPin, clockPin, MSBFIRST, numerot[0]);
  digitalWrite(latchPin, HIGH);
  delay(150);
}