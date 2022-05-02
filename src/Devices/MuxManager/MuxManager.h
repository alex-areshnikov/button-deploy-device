#ifndef mux_manager_h
#define mux_manager_h

#include <CD74HC4067.h>

class MuxManager {
	private:		
		static const int WAKEUP_INTERVAL_MS = 300;

		CD74HC4067 *muxController;
		
		int currentStep;
		int erredStep;
		bool erred;		
		
		void set(int);
		
	public:
		static const int READY_STEP = 0;
		static const int FINGERPRINT_SCAN_STEP = 1;
		static const int ACCESS_GRANTED_STEP = 2;
		static const int DEPLOYING_STEP = 3;
		static const int INITIALIZING_STEP = 14;
		static const int ERROR_STEP = 15;

		MuxManager(int, int, int, int);
		
		void step(int, bool = false);		
		int step();		
		void nextStep();
		void prevStep();
		void readyStep();		
		void ledCheck();
		void process();
};

#endif