#ifndef deployer_state_h
#define deployer_state_h

#include <ArduinoJson.h>
#include "../../Devices/MuxManager/MuxManager.h"

class DeployerState {
	private:
		char buffer[512];
		StaticJsonDocument<512> staticJsonDocument;
		JsonObject stateObj;
		JsonObject reportedObj;

	public:
		DeployerState();
		void update(const char*, const char*);
		void update(const char*, int);
		void update(const char*, bool);
		char* jsonState();
};

#endif