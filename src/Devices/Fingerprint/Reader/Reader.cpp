#include "Reader.h"

using namespace Fingerprint;

Reader::Reader(Adafruit_Fingerprint* middleFinger, TFTScreen* screen, void (*successCallback)(int), void (*failureCallback)()) {
	this->scanSuccessCallback = successCallback;
	this->scanFailureCallback = failureCallback;
	this->middleFinger = middleFinger;
	this->screen = screen;
}

void Reader::process() {
  if (middleFinger->getImage() != FINGERPRINT_OK) return;
  if (middleFinger->image2Tz() != FINGERPRINT_OK) return;

  if (middleFinger->fingerFastSearch() == FINGERPRINT_OK) {  
		(*scanSuccessCallback)(middleFinger->fingerID);	
	} else {		
		(*scanFailureCallback)();
	}
};