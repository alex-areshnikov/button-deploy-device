#ifndef tft_screen_h
#define tft_screen_h

#include <string>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "../../Services/Counter/Counter.h"

class TFTScreen {
  private:  
  Adafruit_ST7735 *tft;
  Counter *counter;
    
  char countdownBuffer[2];

  public:
  TFTScreen(int8_t cs, int8_t dc, int8_t rst);
  void initialize();

  void reset();
  void softReset();
  void clearLine();
  void delayedSay(String, float);
  void process();
  void countdownFor(int);
  void renderCountdown();
  void say(String);
  void sayln(String);
  void say(std::string);
  void sayln(std::string);
  void say(char*);
  void sayln(char*);
  void say(const char*);
  void sayln(const char*);
  void say(int);
  void sayln(int);
  void startup();
};

#endif