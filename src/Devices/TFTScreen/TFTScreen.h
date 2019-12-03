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
  void say(String Text);
  void sayln(String Text);
  void say(char* Text);
  void sayln(char* Text);
};
