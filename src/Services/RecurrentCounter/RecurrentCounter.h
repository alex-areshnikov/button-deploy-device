#ifndef Included_RecurrentCounter_H
#define Included_RecurrentCounter_H

#include <Arduino.h>

class RecurrentCounter {
	private:
		unsigned long storedMillis;
		unsigned long recurrentMillis;
	public:
		RecurrentCounter(unsigned long);
		bool isReady();
};

#endif