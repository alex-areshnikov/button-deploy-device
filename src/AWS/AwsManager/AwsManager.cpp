#include "AwsManager.h"

AwsManager::AwsManager() {
	this->wakeupper = new Wakeupper(AWS_WAKEUP_INTERVAL_MS);
};

void AwsManager::setup(void (onMessageCallback)(char*, byte*, unsigned int)) {
	wiFiClientSecure = WiFiClientSecure();
	wiFiClientSecure.setInsecure();
	wiFiClientSecure.setCACert((const uint8_t *)AWS_CERT_CA, strlen_P(AWS_CERT_CA));
	wiFiClientSecure.setCertificate((const uint8_t *)AWS_CERT_CRT, strlen_P(AWS_CERT_CRT));
	wiFiClientSecure.setPrivateKey((const uint8_t *)AWS_CERT_PRIVATE, strlen_P(AWS_CERT_PRIVATE));

	mqttClient = PubSubClient(AWS_IOT_ENDPOINT, 8883, onMessageCallback, wiFiClientSecure);
}

void AwsManager::reconnect() {
	while(!mqttClient.connected()) {
    if(mqttClient.connect(AWS_IOT_DEVICE_NAME)) {
      mqttClient.subscribe(AWS_IOT_UPDATE_DOCUMENTS_TOPIC);      
    } else {
      delay(5000);
    }
	}
}

void AwsManager::process() {
  if (!this->wakeupper->isWakeupTime()) {
    return;
  }

	if(!mqttClient.connected()) {
		reconnect();
	}

	mqttClient.loop();
};

void AwsManager::reportState(char* jsonState) {
	mqttClient.publish(AWS_IOT_UPDATE_TOPIC, jsonState);
}