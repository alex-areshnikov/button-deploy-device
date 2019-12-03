#ifndef Included_Wakeupper_H
#define Included_Wakeupper_H

#ifndef UNIT_TEST
	#include <Arduino.h>
#endif

#ifdef UNIT_TEST
	#include <sys/time.h>
#endif

class Wakeupper {
	private:
		unsigned int ms_wakeup_interval;
		unsigned long last_wakeup_timestamp;
		
		unsigned long currentMillis();

	public:
		Wakeupper(int);
		bool isWakeupTime();
};

#endif