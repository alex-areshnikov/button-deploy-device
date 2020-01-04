#include <ArduinoJson.h>

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
		char* jsonState();
};