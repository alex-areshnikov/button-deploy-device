#include "Reader.h"

using namespace Fingerprint;

Reader::Reader(Adafruit_Fingerprint* middleFinger, TFTScreen* screen, void (*successCallback)(int), void (*failureCallback)()) {
	this->scanSuccessCallback = successCallback;
	this->scanFailureCallback = failureCallback;
	this->middleFinger = middleFinger;
	this->screen = screen;
	wakeupper = new Wakeupper(WAKEUP_INTERVAL_MS);
}

void Reader::process() {
	if(!wakeupper->isWakeupTime()) return;
	
  if (middleFinger->getImage() != FINGERPRINT_OK) return;
  if (middleFinger->image2Tz() != FINGERPRINT_OK) return;

  if (middleFinger->fingerFastSearch() == FINGERPRINT_OK) {  
		(*scanSuccessCallback)(middleFinger->fingerID);
		// found a match!
		// screen->reset();
		// screen->say("ID #"); 
		// screen->say(middleFinger->fingerID); 
		// screen->say(". Confidence = "); 
		// screen->sayln(middleFinger->confidence);		
	} else {		
		(*scanFailureCallback)();
		// screen->sayln("Unrecognized finger");
	}

	// delay(1000);

	// screen->reset();
	// screen->sayln("Hello Decisely");
};