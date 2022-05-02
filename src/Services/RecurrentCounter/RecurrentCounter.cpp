#include "RecurrentCounter.h"

RecurrentCounter::RecurrentCounter(unsigned long ms) {
	storedMillis = millis();
	recurrentMillis = ms;
}

bool RecurrentCounter::isReady() {
	if(storedMillis + recurrentMillis < millis()) {
		storedMillis = millis();
		return true;
	} else { 
		return false; 
	}  
};
