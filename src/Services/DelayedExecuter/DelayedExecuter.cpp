#include "DelayedExecuter.h"

DelayedExecuter::DelayedExecuter() {
	executionRequested = false;
}

void DelayedExecuter::executeRequest(float executeInSeconds) {
	executionRequested = true;
	executeAtTimestamp = (currentMillis() + executeInSeconds * 1000);
};

bool DelayedExecuter::readyToExecute() {
	if(!executionRequested) return false;
	if(executeAtTimestamp > currentMillis()) return false;
	
	return true;
};

void DelayedExecuter::cancel() {
	executionRequested = false;
};

bool DelayedExecuter::isExecutionRequested() {
	return executionRequested;
};

int DelayedExecuter::secondsToExecution(){
	return ((executeAtTimestamp - currentMillis()) / 1000) + 1;
}

#ifndef UNIT_TEST
	unsigned long DelayedExecuter::currentMillis() {
		return(millis());
	}
#endif

#ifdef UNIT_TEST
	unsigned long DelayedExecuter::currentMillis() {
		struct timeval tp;
		gettimeofday(&tp, NULL);
		unsigned long ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		return(ms);
	}
#endif