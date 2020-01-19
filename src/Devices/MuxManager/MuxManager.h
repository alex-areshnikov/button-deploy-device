#include <CD74HC4067.h>
#include "../../Services/Wakeupper/Wakeupper.h"
#include "../../Services/DelayedExecuter/DelayedExecuter.h"

class MuxManager {
	private:		
		static const int WAKEUP_INTERVAL_MS = 300;

		CD74HC4067 *muxController;
		Wakeupper *wakeupper;
		DelayedExecuter *delayedStepExecuter;
		int currentStep;
		int erredStep;
		bool erred;		

		int requestedStep;
		bool requestedErred;		
		
		void set(int);
		
	public:
		static const int READY_STEP = 0;
		static const int INITIALIZING_STEP = 14;
		static const int ERROR_STEP = 15;

		MuxManager(int, int, int, int);
		
		void step(int, bool = false);
		void delayedStep(float, int, bool = false);
		void c(int, bool, float);
		void nextStep();
		void prevStep();
		void readyStep();		
		void ledCheck();
		void process();

		bool isReadyStep();
};