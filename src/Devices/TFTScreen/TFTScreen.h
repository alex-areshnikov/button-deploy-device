#ifndef tft_screen_h
#define tft_screen_h

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

class TFTScreen {
  private:  
  Adafruit_ST7735 *tft;

  public:
  TFTScreen(int8_t cs, int8_t dc, int8_t rst);
  void initialize();

  void reset();
  void softReset();
  void clearLine();
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
};

#endif