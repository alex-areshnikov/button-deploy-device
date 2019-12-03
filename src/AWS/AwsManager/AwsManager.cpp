#include "AwsManager.h"

AwsManager::AwsManager() {
	this->wakeupper = new Wakeupper(AWS_WAKEUP_INTERVAL_MS);

	wiFiClientSecure = WiFiClientSecure();
	wiFiClientSecure.setInsecure();
	wiFiClientSecure.setCACert((const uint8_t *)AWS_CERT_CA, strlen_P(AWS_CERT_CA));
	wiFiClientSecure.setCertificate((const uint8_t *)AWS_CERT_CRT, strlen_P(AWS_CERT_CRT));
	wiFiClientSecure.setPrivateKey((const uint8_t *)AWS_CERT_PRIVATE, strlen_P(AWS_CERT_PRIVATE));

	mqttClient = PubSubClient(wiFiClientSecure);
	mqttClient.setServer(AWS_IOT_ENDPOINT, 8883);
};

bool AwsManager::connect(void (*connectingCallback)()) {
	int retries = 0;
	while (!mqttClient.connect(AWS_IOT_DEVICE_NAME) && retries < AWS_MAX_RECONNECT_TRIES) {			
		(*connectingCallback)();
		retries++;
	}

	return(mqttClient.connected());
};

void AwsManager::process() {
  if (!this->wakeupper->isWakeupTime()) {
    return;
  }

	mqttClient.loop();
};

void AwsManager::reportState(char* jsonState) {
	mqttClient.publish(AWS_IOT_TOPIC, jsonState);
}