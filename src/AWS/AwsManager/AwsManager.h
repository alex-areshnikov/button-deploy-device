#ifndef aws_manager_h
#define aws_manager_h

#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "../certs.h"
#include "../constants.h"

class AwsManager {
	private:
		WiFiClientSecure wiFiClientSecure;
		PubSubClient mqttClient;

	public:
		AwsManager();
		void setup(void (onMessageCallback)(char*, byte*, unsigned int));
		void process();
		void reconnect();
		void reportState(char*);
};

#endif