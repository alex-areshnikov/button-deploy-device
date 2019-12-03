#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "../../Services/Wakeupper/Wakeupper.h"
#include "../certs.h"
#include "../constants.h"

class AwsManager {
	private:
		Wakeupper *wakeupper;
		WiFiClientSecure wiFiClientSecure;
		PubSubClient mqttClient;

	public:
		AwsManager();
		void process();
		bool connect(void (*connectingCallback)());
		void reportState(char*);	
};