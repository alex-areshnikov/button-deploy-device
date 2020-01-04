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
		void setup(void (onMessageCallback)(char*, byte*, unsigned int));
		void process();
		void reconnect();
		void reportState(char*);
};