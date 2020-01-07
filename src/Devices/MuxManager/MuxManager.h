#include <CD74HC4067.h>
#include "../../Services/Wakeupper/Wakeupper.h"

class MuxManager {
	private:		
		static const int WAKEUP_INTERVAL_MS = 300;
		static const int READY_STEP = 0;
		static const int ERROR_STEP = 15;

		CD74HC4067 *muxController;
		Wakeupper *wakeupper;
		int currentStep;
		int erredStep;
		bool erred;		
		
		void set(int);
		
	public:
		MuxManager(int, int, int, int);
		
		void step(int, bool = false);
		void nextStep();
		void prevStep();
		void readyStep();
		void ledCheck();
		void process();
};