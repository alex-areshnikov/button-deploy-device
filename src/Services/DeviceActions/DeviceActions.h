#include "../../Devices/TFTScreen/TFTScreen.h"
#include "../../Devices/MuxManager/MuxManager.h"
#include "../DelayedExecuter/DelayedExecuter.h"
#include "../DeployerState/DeployerState.h"
#include "../../AWS/AwsManager/AwsManager.h"

class DeviceActions {
	private:
		TFTScreen* screen;
		MuxManager* muxManager;
		DelayedExecuter *delayedExecuter;
		DeployerState *deployerState;
		AwsManager *awsManager;

		int requestedAction;

		void executeRequestedAction();
		void ready();
		void accessGranted();
		void accessDenied();
	public:
		static const uint8_t EXECUTE_IMMEDIATELY = -1;

		static const uint8_t DO_NOTHING = 0;
		static const uint8_t READY = 1;
		static const uint8_t ACCESS_GRANTED = 2;
		static const uint8_t ACCESS_DENIED = 3;
		

		DeviceActions(TFTScreen*, MuxManager*, AwsManager*, DeployerState*);

		void execute(uint8_t, int = EXECUTE_IMMEDIATELY);
		void process();
};