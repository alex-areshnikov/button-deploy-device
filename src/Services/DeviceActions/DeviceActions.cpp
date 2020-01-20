#include "DeviceActions.h"

DeviceActions::DeviceActions(TFTScreen *screen, MuxManager *muxManager, AwsManager *awsManager, DeployerState *deployerState) {
	delayedExecuter = new DelayedExecuter();

	this->screen = screen;
	this->muxManager = muxManager;
	this->deployerState = deployerState;
	this->awsManager = awsManager;

	requestedAction = DO_NOTHING;
}

void DeviceActions::execute(uint8_t action, int seconds) {
	requestedAction = action;

	if(seconds == EXECUTE_IMMEDIATELY) {
		executeRequestedAction();
	} else {
		screen->countdownFor(seconds);
		delayedExecuter->executeRequest(seconds);
	}
}

void DeviceActions::executeRequestedAction() {
	switch(requestedAction) {
		case DO_NOTHING: 
			break;
		case READY:
			ready();
			break;
		case ACCESS_GRANTED:
			accessGranted();
			break;
		case ACCESS_DENIED:
			accessDenied();
			break;
		default:			
			break;
	}	

	requestedAction = DO_NOTHING;
}

void DeviceActions::process() {
	if(delayedExecuter->readyToExecute()) {		
		delayedExecuter->cancel();
    executeRequestedAction();		
  }
}

void DeviceActions::ready() {
	muxManager->step(MuxManager::READY_STEP);
	deployerState->update("step", MuxManager::READY_STEP);
	deployerState->update("access_granted", false);
	deployerState->update("error", false);
	deployerState->update("enrolling", nullptr);
	deployerState->update("enrolled", nullptr);
	deployerState->update("finger_id", nullptr);
	awsManager->reportState(deployerState->jsonState());
	
	screen->reset();
	screen->sayln("Hello Decisely");
}

void DeviceActions::accessGranted() {
	muxManager->step(MuxManager::FINGERPRINT_SCAN_STEP);
  delay(200);

  muxManager->step(MuxManager::ACCESS_GRANTED_STEP);
	deployerState->update("step", MuxManager::ACCESS_GRANTED_STEP);
	deployerState->update("access_granted", true);
	awsManager->reportState(deployerState->jsonState());

  screen->reset();
  screen->sayln("Access Granted!");
}

void DeviceActions::accessDenied() {
	muxManager->step(MuxManager::FINGERPRINT_SCAN_STEP, true);
	deployerState->update("step", MuxManager::FINGERPRINT_SCAN_STEP);
	deployerState->update("error", true);
	awsManager->reportState(deployerState->jsonState());

  screen->reset();
  screen->sayln("Access Denied");
  screen->sayln("Unrecognized finger");  
}