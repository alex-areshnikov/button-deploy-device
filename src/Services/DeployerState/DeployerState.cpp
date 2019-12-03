#include "DeployerState.h"

DeployerState::DeployerState() {
  stateObj = staticJsonDocument.createNestedObject("state");
	reportedObj = stateObj.createNestedObject("reported");
};

void DeployerState::update(const char* key, const char* value) {
	reportedObj[key] = value;
};

char* DeployerState::jsonState() {
	serializeJson(staticJsonDocument, buffer);
	return(buffer);
};