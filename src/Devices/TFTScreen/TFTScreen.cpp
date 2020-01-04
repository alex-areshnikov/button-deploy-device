#include "TFTScreen.h"

TFTScreen::TFTScreen(int8_t cs, int8_t dc, int8_t rst) {
  this->tft = new Adafruit_ST7735(cs, dc, rst);
}

void TFTScreen::initialize() {  
  tft->initR(INITR_BLACKTAB);
  //tft.setTextWrap(false); // Allow text to run off right edge
  this->reset();
  tft->setRotation(1);
  tft->setTextColor(ST77XX_MAGENTA, ST77XX_BLACK);
  tft->setTextSize(1);
}

void TFTScreen::reset() {
  tft->fillScreen(ST77XX_BLACK);
  tft->setCursor(0, 0);
}

void TFTScreen::softReset() {
  tft->setCursor(0, 0);
}

void TFTScreen::say(String text) {
    tft->print(text);
}

void TFTScreen::sayln(String text) {
    tft->println(text);
}

void TFTScreen::say(std::string text) {
    say(&text[0]);
}

void TFTScreen::sayln(std::string text) {
    sayln(&text[0]);
}

void TFTScreen::say(char* text) {
    say(String(text));
}

void TFTScreen::sayln(const char* text) {
    sayln(String(text));
}

void TFTScreen::say(const char* text) {
    say(String(text));
}

void TFTScreen::sayln(char* text) {
    sayln(String(text));
}

void TFTScreen::say(int text) {
    say(String(text));
}

void TFTScreen::sayln(int text) {
    sayln(String(text));
}