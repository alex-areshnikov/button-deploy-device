#include "Initializer.h"

using namespace Fingerprint;

Initializer::Initializer(Adafruit_Fingerprint* middleFinger, TFTScreen* screen) {
	this->middleFinger = middleFinger;
	this->screen = screen;
}

void Initializer::call() {
	screen->reset();
	middleFinger->begin(57600);

  if (middleFinger->verifyPassword()) {
    screen->sayln("Fingerprint sensor OK!");    
  } else {
    screen->sayln("Did not find fingerprint sensor.");
		screen->sayln("Check connections and restart the Deployer.");
    while (1) { delay(1); }
  }

  middleFinger->getTemplateCount();
  screen->say("\nKnows "); 
	screen->say(middleFinger->templateCount); 
	screen->say(" middle fingers");
	
	delay(2000);
};