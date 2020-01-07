#include "Wakeupper.h"

Wakeupper::Wakeupper(int msWakeupInterval) {
	this->msWakeupInterval = msWakeupInterval;
	this->lastWakeupTimestamp = currentMillis();
}

bool Wakeupper::isWakeupTime() {
	unsigned long msSinceWakeup = currentMillis() - lastWakeupTimestamp;

	if(msSinceWakeup >= msWakeupInterval) {
		lastWakeupTimestamp = currentMillis();
		return(true);
	}

	return(false);
}

#ifndef UNIT_TEST
	unsigned long Wakeupper::currentMillis() {
		return(millis());
	}
#endif

#ifdef UNIT_TEST
	unsigned long Wakeupper::currentMillis() {
		struct timeval tp;
		gettimeofday(&tp, NULL);
		unsigned long ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		return(ms);
	}
#endif