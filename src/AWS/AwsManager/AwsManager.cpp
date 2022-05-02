#include "AwsManager.h"

AwsManager::AwsManager() {
	wiFiClientSecure = WiFiClientSecure();	
	mqttClient = PubSubClient(wiFiClientSecure);
};

void AwsManager::setup(void (onMessageCallback)(char*, byte*, unsigned int)) {	
	wiFiClientSecure.setInsecure();
	wiFiClientSecure.setCACert((const uint8_t *)AWS_CERT_CA, strlen_P(AWS_CERT_CA));
	wiFiClientSecure.setCertificate((const uint8_t *)AWS_CERT_CLIENT, strlen_P(AWS_CERT_CLIENT));
	wiFiClientSecure.setPrivateKey((const uint8_t *)AWS_CERT_PRIVATE, strlen_P(AWS_CERT_PRIVATE));
		
	mqttClient.setServer(AWS_IOT_ENDPOINT, 8883);
  	mqttClient.setCallback(onMessageCallback);	
}

void AwsManager::reconnect() {
	while(!mqttClient.connected()) {
		if(mqttClient.connect(AWS_IOT_DEVICE_NAME)) {
			//mqttClient.subscribe(AWS_IOT_UPDATE_DOCUMENTS_TOPIC);  
			mqttClient.loop();   
		} else {
			delay(2000);
		}
	}
}

void AwsManager::process() {
	if(!mqttClient.connected()) {
		reconnect();
	}

	mqttClient.loop();
};

void AwsManager::reportState(char* jsonState) {
	//mqttClient.publish(AWS_IOT_UPDATE_TOPIC, jsonState);
}