#include "Reader.h"

using namespace Fingerprint;

Reader::Reader(Adafruit_Fingerprint* middleFinger, TFTScreen* screen) {
	this->middleFinger = middleFinger;
	this->screen = screen;
	wakeupper = new Wakeupper(WAKEUP_INTERVAL_MS);
}

void Reader::process() {
	if(!wakeupper->isWakeupTime()) return;
	
  if (middleFinger->getImage() != FINGERPRINT_OK) return;
  if (middleFinger->image2Tz() != FINGERPRINT_OK) return;

	screen->reset();

  if (middleFinger->fingerFastSearch() == FINGERPRINT_OK) {  
		// found a match!
		screen->reset();
		screen->say("ID #"); 
		screen->say(middleFinger->fingerID); 
		screen->say(". Confidence = "); 
		screen->sayln(middleFinger->confidence);		
	} else {		
		screen->sayln("Unrecognized finger");
	}

	delay(1000);

	screen->reset();
	screen->sayln("Hello Decisely");
};