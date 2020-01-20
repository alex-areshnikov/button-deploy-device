#include "Counter.h"

Counter::Counter() {
	// Do nothing
}

void Counter::countSeconds(int seconds) {
	countToTimestamp = (currentMillis() + seconds * 1000);
};

int Counter::currentSecond() {
	if(countToTimestamp < currentMillis()) return 0;
	return ((countToTimestamp - currentMillis()) / 1000) + 1;
};

#ifndef UNIT_TEST
	unsigned long Counter::currentMillis() {
		return(millis());
	}
#endif

#ifdef UNIT_TEST
	unsigned long Counter::currentMillis() {
		struct timeval tp;
		gettimeofday(&tp, NULL);
		unsigned long ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		return(ms);
	}
#endif