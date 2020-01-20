#include "DeployerState.h"

DeployerState::DeployerState() {
  stateObj = staticJsonDocument.createNestedObject("state");
	reportedObj = stateObj.createNestedObject("reported");
	stateObj["desired"] = nullptr;

	// TODO: reportedObj["button"] = ButtonParser::NO_BUTTON;
	reportedObj["button"] = "no_button_pressed";
  reportedObj["step"] = MuxManager::READY_STEP;
	reportedObj["access_granted"] = false;

  reportedObj["error"] = false;
};

void DeployerState::update(const char* key, const char* value) {
	reportedObj[key] = value;
};

void DeployerState::update(const char* key, int value) {
	reportedObj[key] = value;
};

void DeployerState::update(const char* key, bool value) {
	reportedObj[key] = value;
};

char* DeployerState::jsonState() {
	serializeJson(staticJsonDocument, buffer);
	return(buffer);
};