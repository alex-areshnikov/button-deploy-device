#ifndef Included_Delayed_Executer_H
#define Included_Delayed_Executer_H

#ifndef UNIT_TEST
	#include <Arduino.h>
#endif

#ifdef UNIT_TEST
	#include <sys/time.h>
#endif

class DelayedExecuter {
	private:
		unsigned long executeAtTimestamp;
		bool executionRequested;
		
		unsigned long currentMillis();

	public:
		DelayedExecuter();

		void executeRequest(float);
		bool readyToExecute();
		bool isExecutionRequested();
		int secondsToExecution();
		void cancel();
};

#endif