#include <CD74HC4067.h>

class MuxManager {
	private:		
		int currentStep;
		CD74HC4067 *muxController;
		
	public:
		MuxManager(int, int, int, int);

		void step(int);
		void nextStep();
		void prevStep();
		void readyStep();
};