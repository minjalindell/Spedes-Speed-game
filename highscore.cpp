#include "highscore.h"
#include <EEPROM.h>
#include "arduino.h"

const int latchPin2 = 12; // Latch-pinni
const int clockPin2 = 13; // Clock-pinni
const int dataPin2 = 9; // Data-pinni

// Näytön numerot digitaalisina segmentteinä (0-9)
uint8_t numerot2[] ={
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


int highscoreOsoite = 0; //Asettaa muuttujan

int highScore; //Asettaa muuttujan
int laske;

void getPelinTulos(int currentScore) {
  if (currentScore > highScore) {  //Jos currentscore on suurempi kuin highscore:
    highScore = currentScore;      //Asettaa currentscoren highscoren tilalle
    EEPROM.write(0, highScore); //Asettaa päivitetyn highscoren EEPROMIIN
    Serial.print("Uusi highscore: "); //Printtaa uuden tuloksen
    Serial.println(highScore); //Printtaa päivitetyn tuloksen
    laske = highScore;
  }
}

void naytaNumerot2(){
  digitalWrite(latchPin2, LOW);
  shiftOut(dataPin2, clockPin2, MSBFIRST, numerot2[highScore / 10]); // Toisen näytön numeron esitys
  shiftOut(dataPin2, clockPin2, MSBFIRST, numerot2[highScore % 10]); // Ensimmäisen näytön numeron esitys
  digitalWrite(latchPin2, HIGH);
  delay(150);// Pieni viive näytöllä
}


void nollaaHighScore(){
  EEPROM.write(0, 0);   //Muistipaikkaan 0 kirjoitetaan 0
  highScore = 0;
  laske = 0;
}


void initializeDisplay2(){    // Asetetaan pinnit OUTPUT -tilaan
  pinMode(latchPin2, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(dataPin2, OUTPUT);
  //byte highScore = EEPROM.read(highscoreOsoite);
}


void Put_pelin_tulos(){
  highScore = EEPROM.read(0);   //highScore = EEPROM muistipaikasta 0 luettu arvo
  shiftOut(dataPin2, clockPin2, MSBFIRST, EEPROM.read(highScore)); // Toisen näytön numeron esitys
  shiftOut(dataPin2, clockPin2, MSBFIRST, EEPROM.read(highScore)); // Ensimmäisen näytön numeron esitys
  digitalWrite(latchPin2, HIGH);  
}