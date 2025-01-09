#ifndef HIGHSCORE_H
#define HIGHSCORE_H
#include <arduino.h>


extern int highscoreOsoite; //Asettaa globaalin muuttujan
extern int currentScore;  //Asettaa globaalin muuttujan 

extern int highScore; //Asettaa globaalin muuttujan

//void nollaaHighScoreNaytto();
void initializeDisplay2();
void naytaNumerot2();
void nollaaHighScore();

void Put_pelin_tulos();

void getPelinTulos(int currentScore);

#endif // HIGHSCORE_H
