#ifndef Included_Counter_H
#define Included_Counter_H

#ifndef UNIT_TEST
	#include <Arduino.h>
#endif

#ifdef UNIT_TEST
	#include <sys/time.h>
#endif

class Counter {
	private:
		unsigned long countToTimestamp;

		unsigned long currentMillis();

	public:
		Counter();
		void countSeconds(int);
		int currentSecond();
};

#endif