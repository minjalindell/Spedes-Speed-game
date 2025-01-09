#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
#include "highscore.h"
#include "Mario.h"
#include "RickRoll.h"
#include <EEPROM.h>
#include <avr/io.h>

//Määritellään muuttujat
int randomLed; // Satunnaisesti valittu LEDin indeksi
byte userIndex = 0; // Käyttäjän painallusten indeksi
int led[100]; // Vektori LEDien tallentamiseksi
int userPush[100]; // Käyttäjän painallukset
byte ledIndex = 0; // LEDin indeksi
byte index = 0; // Nykyinen vertailuindeksi
int counters = 0; // Oikeiden painallusten laskuri


// Napinpainalluspiinien määritykset
int pin2 = 2;
int pin3 = 3;
int pin4 = 4;
int pin5 = 5;
int pin6 = 6;
const int buttons[] = {2, 3, 4, 5, 6};

int lastButtonPress = 5; // Viimeisin napinpainallus
long compareMatchValue = 15624; // Vertailuarvo keskeytysten määrälle
extern int currentScore = 0; // Nykyinen pistemäärä
volatile byte state = LOW; // Tilamuuttuja

extern volatile bool buttonPressed[5] = {false}; // Taulukko painiketiloille
volatile bool buttonPush[1] = {false}; // Tilamuuttuja painallusten tallennukselle
const long interval = 1000; // Aikaväli millisekuntteina
unsigned long previousMillis = 0; // Viimeisin aika kun tila tarkistettiin
volatile bool startGame[1] = {false}; // Tilamuuttuja pelin aloittamiselle
int count = 0; // Laskuri pelin tilalle

void setup(){
  // Alustaa sarjaportin
  Serial.begin(9600);
  // Alustaa highscore näytöt
  initializeDisplay2();
  // Alustaa timerin
  initializeTimer();  
  // Alustaa napit
  initButtonsAndButtonInterrupts();
  // Alustaa näytöt
  initializeDisplay();
  // Asettaa LEDit päälle
  setAllLeds();
  // Alustaa LEDit
  leds();
}

void loop(){
  for(int i = 2; i <= 5; ++i){
    // Jos nappi 6 on painettu ja count on 0
    if(buttonPressed[4] && count == 0){
      count = 1;
      Serial.println(count);
    }

    // Switch-case rakenteella hallitaan pelin eri tiloja
    switch(count){
      case 1:// Aseta count arvoksi 2 suorita nolla-funktio
      count = 2;
      // Suorita nolla-funktio
      nolla();
      // Lyhyt delay
      delay(50);
      // Käynnistä peli
      startTheGame();
      Serial.println("Aloitetaan");
      break;



      case 2:
      // Tyhjä case
      break;

      case 3:
      Serial.println("Kesk...");
      // Nollaa pelin tilat ja muuttujat
      currentScore = 0;
      lastButtonPress = 5;
      index = 0;
      userIndex = 0;
      ledIndex = 0;
      buttonPush[0] = false;
      // Tyhjennetään taulukot
      led[100];
      userPush[100];
      count = 0;
      counters = 0;
      // Suoritetaan nollaus-funktio
      nollaus();
      // Suoritetaan nollaus-funktio
      nolla();
      // Asetetaan kaikki LEDit päälle
      setAllLeds();
      // Aseta compareMatchValue ja 0CR1A alkuperäisiin arvoihinsa
      compareMatchValue = 15624;
      OCR1A = compareMatchValue;
      // Suorita leds-funktio
      leds();
      Serial.println("Keskeytetty");
      // Sammuta peli
      startGame[0] = false;
      break;
    }
    // Jos peli on käynnissä
    if(startGame[0]){
      // Tarkista onko nappia painettu
      if (buttonPressed[i - 2]) {
        Serial.println("loop");
        // Suoritetaan checkGame-funktio
        checkGame(); 
        // Asetetaan napinpainallus pois päältä
        buttonPressed[i- 2] = false;
      }
    }
  }

  // Suorittaa tarvittavat funktiot
  naytaNumerot();
  buttonISR();
  getPelinTulos(currentScore);
  Put_pelin_tulos();

  // Aseta napit ja napin keskeytykset uudelleen
  initButtonsAndButtonInterrupts(); 
  // Suorita naytaNumerot2-funktio
  naytaNumerot2();

}

ISR(PCINT2_vect) {
  // Pieni viive kytkinvärähtelyä varten
   delay(50);

    // Käy läpi pinnit 2-6
    for (int i = 2; i <= 6; ++i) {
      // Tarkista, onko nappia painettu
      if (!(PIND & (1 << i))) {
          // Asettaa painikkeen tilan "painettu"-tilaan
          buttonPressed[i - 2] = true;
      }
    }

    // Tarkistaa onko peli käynnissä
    if(startGame[0]){
      // Tarkistaa onko käynnistysnappi painettu
      if(buttonPressed[4]){
          Serial.println("Start");
          // Asettaa laskuriarvon ja käynnistää pelin
          count = 3;
          startTheGame();
          buttonPressed[4] = false;   
      }
    }
}

ISR(TIMER1_COMPA_vect)
{  
  // Tyhjennä kaikki LEDit aloittaaksesi puhtaalta pohjaltaclearAllLeds();
  clearAllLeds();

  // Tarkista, onko peli tällä hetkellä aktiivinen
  if(startGame[0]){
    // Aseta LED senhetkisen indeksin mukaisesti led-taulukosta
    Serial.print("LED ");
    setLed(led[ledIndex]);
    Serial.println("Toimii");

    // Kasvata LED-indeksiä seuraavaa iteraatiota varten 
    ledIndex++;
    
    // Resetoi ledIndex, jos se ylittää taulukon koon
    if(ledIndex == 100){
      ledIndex = 0;
    }
  }
}

void initializeTimer(void)
{
  // Poista keskeytykset käytöstä
  cli();
  // Nollaa asetukset
  TCCR1A = 0;
  TCCR1B = 0;
  // Aseta tila CTC-tilaan (Clear Timer on Compare Match)
  TCCR1B |=(1 << WGM12);
  // Aseta jakajaparametrit (prescaler) arvoon 1024
  TCCR1B |=(1 << CS12) | (1 << CS10);

  // Aseta keskeytys päälle verrannollisen arvon saavuttaessa
  TIMSK1 |= (1 << OCIE1A);
  // Aseta verrannollinen arvo (compare match value) OCR1A-rekisteriin
  OCR1A = compareMatchValue;

  // Ota keskeytykset käyttöön
  sei();
}


 void time(){
    // Hakee nykyisen ajan millisekuntteina
    unsigned long currentMillis = millis();
    
    // Tarkistaa, onko kulunut aika riittävä verrattuna edelliseen aikaan
    if (currentMillis - previousMillis >= interval){
      // Päivittää edellisen ajan
      previousMillis = currentMillis;
    }
 }


void checkGame()
{
  delay(100);
 // Tarkistetaan napin 0 tila
  if(buttonPressed[0]){
    Serial.println("0 Toimii!!!!!");
    lastButtonPress = 0;  
    userPush[userIndex] = lastButtonPress;
    buttonPressed[0] = false;
    userIndex++;
    buttonPush[0] = true;
  }
 // Tarkistetaan napin 1 tila
  if(buttonPressed[1]){
    Serial.println("1 Toimii!!!!!");
    lastButtonPress = 1;
    userPush[userIndex] = lastButtonPress;
    userIndex++;
    buttonPush[0] = true;
    buttonPressed[1] = false;
  }
 // Tarkistetaan napin 2 tila
  if(buttonPressed[2]){
    Serial.println("2 Toimii!!!!!");
    lastButtonPress = 2; 
    userPush[userIndex] = lastButtonPress;
    userIndex++;
    buttonPush[0] = true;
    buttonPressed[2] = false;
  }
 // Tarkistetaan napin 3 tila
  if(buttonPressed[3]){
    Serial.println("3 Toimii!!!!!");
    lastButtonPress = 3;
    userPush[userIndex] = lastButtonPress;
    userIndex++;
    buttonPush[0] = true;
    buttonPressed[3] = false;
  }
  // Jos nappia 4 painettu, resetoidaan peli
  if(buttonPressed[4]){
    count = 3;
    Serial.println("Nappi 4");
    Serial.print("count: ");
    Serial.println(count);
    buttonPressed[4] = false;
  }

  // Jos nappia painettu, verrataan se Ledien kanssa
  if(buttonPush[0]){
    // Jos nappi vastaa LEDiä nostetaan pistettä ja jatketaan peliä
    if(led[index] == userPush[index]){
      Serial.println("OIKEIN!!!!!");
      Serial.print("Nappi: ");
      Serial.println(userPush[index]);
      Serial.print("Led: ");
      Serial.println(led[index]);
      Serial.print("Highscore: ");
      Serial.println(highScore);
      Serial.println(counters);

      counters++;
      laskuri();
      currentScore = currentScore + 1;
      Serial.print("Highscore: ");
      Serial.println(highScore);
      Serial.print("count: ");
      Serial.println(count);

      lastButtonPress = 5;
      buttonPush[0] = false;
      index++;
      Serial.println("Jatkuu...");
    }
    // Jos nappi ei vastaa LEDiä, peli resetoidaan
    else if(led[index] != userPush[index]){
      //nollaaHighScore();
      clearAllLeds();
      Serial.println("Väärin!!");
      Serial.print("Käyttäjä: ");
      Serial.println(userPush[index]);
      Serial.print("Kone: ");
      Serial.println(led[index]);
      Serial.print("Highscore: ");
      Serial.println(highScore);

      compareMatchValue = 15624;
      OCR1A = compareMatchValue;

      currentScore = 0;
      lastButtonPress = 5;
      index = 0;
      userIndex = 0;
      ledIndex = 0;
      startGame[0] = false;
      buttonPush[0] = false;
      led[100];
      userPush[100];
      count = 0;
      counters = 0;
      nollaus();
      setAllLeds();
      buttonPressed[0] = false;
      buttonPressed[1] = false;
      buttonPressed[2] = false;
      buttonPressed[3] = false;
      RickRoll();
      Serial.println("Uudestaan");
    }

    // Kymmenen oikean napin painalluksen jälkeen lisätään keskeytystaajuutta
    if(counters == 10){
      Serial.println("Vrooom");
      counters = 0;
      Serial.println("Counter 0");
      // Lisää keskeytystaajuutta 20 prosenttia
      compareMatchValue = compareMatchValue / 1.15;
      Serial.println("Arvo muutettu");
      noInterrupts();
      OCR1A = compareMatchValue;
      Serial.println(compareMatchValue);
      interrupts();
    }


    //Jos saavutetaan 100 pistettä peli lopetetaan
    if(currentScore == 100){
      clearAllLeds();
      Serial.println("VOITIT!!");
      Serial.print("Käyttäjä: ");
      Serial.println(userPush[index]);
      Serial.print("Kone: ");
      Serial.println(led[index]);
      Serial.print("Highscore: ");
      Serial.println(highScore);

      compareMatchValue = 15624;
      OCR1A = compareMatchValue;

      currentScore = 0;
      lastButtonPress = 5;
      index = 0;
      userIndex = 0;
      ledIndex = 0;
      startGame[0] = false;
      buttonPush[0] = false;
      led[100];
      userPush[100];
      count = 0;
      counters = 0;
      nollaus();
      setAllLeds();
      delay(200);
      clearAllLeds();
      delay(200);
      setAllLeds();
      delay(200);
      delay(200);
      setAllLeds();
      delay(200);
      clearAllLeds();
      buttonPressed[0] = false;
      buttonPressed[1] = false;
      buttonPressed[2] = false;
      buttonPressed[3] = false;
      MaarioWohoo();
      Serial.println("Älä tee uudestaan >:(");
    }
  }
}

void initializeGame(){
  // Kutsuu checkGame-funktiota ja initializeLeds-funktiota
  checkGame();
  initializeLeds();
}

void startTheGame(){
  // Asettaa startGame-taulukon todeksi ja napin 4 painalluksen epätodeksi
  startGame[0] = true;
  buttonPressed[4] = false;

  // Kutsuu initializeTimer-funktiota ja initializeGame-funktiota
  initializeTimer();
  initializeGame();

}
void blink(){
  // Vaihda tilaa (true -> false tai false -> true)
  state = !state;
}
void leds(){
    for(int i = 0;i <= 100; i++ ){
    int sopiva_luku_loytyi=0;
    while(sopiva_luku_loytyi==0){
      // Arvotaan random luku 0 ja 3 välillä
      randomLed=random(0,4);
      // Jos ollaan ensimmäisessä elementissä, kaikki käy
      if(i==0){
        sopiva_luku_loytyi=1;
      } else{
        // Tarkistetaan, että arvottu luku ei ole sama kuin edellinen
        if(randomLed!=led[i-1]){
        sopiva_luku_loytyi=1;
        }
      }
    }
    // Laitetaan luku vektorin sisälle
    led[i]=randomLed;
  }
}

