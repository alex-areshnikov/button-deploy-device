#include "Enroller.h"

using namespace Fingerprint;

Enroller::Enroller(Adafruit_Fingerprint* middleFinger, TFTScreen* screen, int enrollId) {
	this->middleFinger = middleFinger;
	this->screen = screen;
	this->enrollId = enrollId;
};

void Enroller::call() {
	screen->reset();
	screen->say("Enrolling #");
	screen->sayln(enrollId);
	screen->sayln("Scan your finger");

	scanFinger();
	if(convertImage(1) != FINGERPRINT_OK)  { reportError(); return; }
	
	screen->sayln("Remove finger");
	waitNoFinger();
	
	screen->sayln("Place same finger again");
	scanFinger();
	if(convertImage(2) != FINGERPRINT_OK)  { reportError(); return; }

	if(checkImages() != FINGERPRINT_OK) { reportError(); return; }
	if(stroreFingerprint() != FINGERPRINT_OK)  { reportError(); return; }
	
	screen->say("\nSuccessfully Enrolled #");
	screen->sayln(enrollId);

	delay(5000);

	screen->reset();
};

void Enroller::scanFinger() {
	uint8_t result = -1;
	
	while (result != FINGERPRINT_OK) {	
    result = middleFinger->getImage();

    switch (result) {
			case FINGERPRINT_OK:
				break;
			case FINGERPRINT_NOFINGER:      
				break;
			case FINGERPRINT_PACKETRECIEVEERR:
				screen->sayln("Communication error");
				break;
			case FINGERPRINT_IMAGEFAIL:
				screen->sayln("Imaging error");
				break;
			default:
				screen->sayln("Unknown error");
				break;
    }

		delay(200);
  }
};

uint8_t Enroller::convertImage(int sequence) {
	uint8_t result = middleFinger->image2Tz(sequence);

  switch (result) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      screen->sayln("Image too messy");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      screen->sayln("Communication error");
      break;
    case FINGERPRINT_FEATUREFAIL:
      screen->sayln("Could not find fingerprint features");
      break;
    case FINGERPRINT_INVALIDIMAGE:
      screen->sayln("Could not find fingerprint features");
      break;
    default:
      screen->sayln("Unknown error");
      break;
  }

	return result;
};

void Enroller::waitNoFinger() {
	uint8_t result;
	while (result != FINGERPRINT_NOFINGER) {
    result = middleFinger->getImage();
		delay(500);
  }
};

uint8_t Enroller::checkImages() {
	uint8_t result = middleFinger->createModel();

	if (result == FINGERPRINT_OK) {
    screen->sayln("Prints matched!");
  } else if (result == FINGERPRINT_PACKETRECIEVEERR) {
    screen->sayln("Communication error");    
  } else if (result == FINGERPRINT_ENROLLMISMATCH) {
    screen->sayln("Fingerprints did not match");
  } else {
    screen->sayln("Unknown error");
  }  

	return result;
};

uint8_t Enroller::stroreFingerprint() {
	uint8_t result = middleFinger->storeModel(enrollId);

	if (result == FINGERPRINT_OK) {
  } else if (result == FINGERPRINT_PACKETRECIEVEERR) {
    screen->sayln("Communication error");
  } else if (result == FINGERPRINT_BADLOCATION) {
    screen->sayln("Could not store in that location");
  } else if (result == FINGERPRINT_FLASHERR) {
    screen->sayln("Error writing to flash");
  } else {
    screen->sayln("Unknown error");
  }   

	return result;
};

void Enroller::reportError() {
	screen->sayln("\nERROR. Enrollment cancelled.");

	delay(5000);

	screen->reset();
};