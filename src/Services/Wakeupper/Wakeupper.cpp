#include "Wakeupper.h"

Wakeupper::Wakeupper(int ms_wakeup_interval) {
	this->ms_wakeup_interval = ms_wakeup_interval;
	this->last_wakeup_timestamp = currentMillis();
}

bool Wakeupper::isWakeupTime() {
	unsigned long ms_since_wakeup = currentMillis() - this->last_wakeup_timestamp;

	if(ms_since_wakeup >= this->ms_wakeup_interval) {
		this->last_wakeup_timestamp = currentMillis();
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