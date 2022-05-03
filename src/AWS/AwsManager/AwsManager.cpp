#include "AwsManager.h"

AwsManager::AwsManager() {
	wiFiClientSecure = WiFiClientSecure();	
	mqttClient = PubSubClient(AWS_IOT_ENDPOINT, 8883, wiFiClientSecure);

	caCert = new BearSSL::X509List(AWS_CERT_CA);
	clientCert = new BearSSL::X509List(AWS_CERT_CLIENT);
	clientKey = new BearSSL::PrivateKey(AWS_CERT_PRIVATE);
}

void AwsManager::setup(void (onMessageCallback)(char*, byte*, unsigned int)) {	
	wiFiClientSecure.setTrustAnchors(caCert);
	wiFiClientSecure.setClientRSACert(clientCert, clientKey);
	wiFiClientSecure.setBufferSizes(2048, 2048);
				
  	mqttClient.setCallback(onMessageCallback);	
}

void AwsManager::reconnect(void (*connectingCallback)(char*, int)) {
	char errBuff[256];
	int errCode;
	
	while(!mqttClient.connected()) {
		if(mqttClient.connect(AWS_IOT_DEVICE_NAME)) {			
			mqttClient.setBufferSize(2048);
			mqttClient.subscribe(AWS_IOT_UPDATE_DOCUMENTS_TOPIC);  
			mqttClient.loop();   
		} else {
			errCode = wiFiClientSecure.getLastSSLError(errBuff, 256);
			(*connectingCallback)(errBuff, errCode);
			delay(2000);
		}
	}
}

void AwsManager::reconnect() {
	while(!mqttClient.connected()) {
		if(mqttClient.connect(AWS_IOT_DEVICE_NAME)) {
			mqttClient.subscribe(AWS_IOT_UPDATE_DOCUMENTS_TOPIC);  
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
}

void AwsManager::reportState(char* jsonState) {
	mqttClient.publish(AWS_IOT_UPDATE_TOPIC, jsonState);
}