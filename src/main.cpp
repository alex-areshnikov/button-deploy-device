#include <Arduino.h>
#include "Devices/TFTScreen/TFTScreen.h"
#include "Devices/Buttons/ButtonsManager/ButtonsManager.h"
#include "Devices/MuxManager/MuxManager.h"
#include "Devices/Wemos/WiFiConnector/WiFiConnector.h"
#include "AWS/AwsManager/AwsManager.h"
#include "Services/DeployerState/DeployerState.h"
#include "Services/MuxDocumentsProcessor/MuxDocumentsProcessor.h"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define TFT_CS     D4
#define TFT_RST    D6                     
#define TFT_DC     D8

TFTScreen screen(TFT_CS, TFT_DC, TFT_RST);
ButtonsManager buttonsManager;
MuxManager muxManager(D0, D1, D2, D3);
WiFiConnector wiFiConnector(WIFI_SSID, WIFI_PASSWORD);
DeployerState deployerState;
AwsManager awsManager;

StaticJsonDocument<1024> awsIotJson;

void ledCheck() {
  for(int i=0; i<15; i++) {
    muxManager.nextStep();
    delay(50);
  }

  for(int i=0; i<15; i++) {
    muxManager.prevStep();
    delay(50);
  }
}

void debugCallback(char* message) {
  screen.reset();
  screen.say(message);
  delay(500);
}

void connectingCallback() {
  screen.say(".");
  delay(500);
}

void subscribeCallback(char* topic, byte* payload, unsigned int length) {
  char trimmedPayload[length];
  strlcpy(trimmedPayload, (char*)payload, length+1);

  DeserializationError deserializationError = deserializeJson(awsIotJson, trimmedPayload);

  if (deserializationError) {
    screen.reset();
    screen.sayln("Incomint message on topic: ");
    screen.sayln(topic);
    screen.sayln("\nDeserialization Error:");
    screen.sayln(deserializationError.c_str());
  } else {
    if(strcmp(topic, AWS_IOT_UPDATE_DOCUMENTS_TOPIC) == 0) {
      MuxDocumentsProcessor muxDocumentsProcessor(&awsIotJson);
      muxDocumentsProcessor.process();

      if(muxDocumentsProcessor.hasValidStep()) {
        muxManager.step(muxDocumentsProcessor.step());
        deployerState.update("step", muxDocumentsProcessor.step());        
      } 

      if(muxDocumentsProcessor.hasDesiredKey()) {
        awsManager.reportState(deployerState.jsonState());     
      }
    }   
  }
}

void buttonsActionCallback(const char* buttonCode) {
  deployerState.update("button", buttonCode);
  awsManager.reportState(deployerState.jsonState());
}

void setupWiFi() {
  screen.say("Connecting to WiFi");
  wiFiConnector.connect(&connectingCallback);

  screen.reset();
  screen.sayln("WiFi connected");
  screen.say("IP address: ");
  screen.sayln(WiFi.localIP().toString());
  screen.say("RSSI: ");
  screen.sayln(WiFi.RSSI());
  delay(2000);
}

void connectMQTT() {
  screen.reset();
  screen.sayln("Connecting to AWS IoT...");

	awsManager.reconnect();

  screen.sayln("AWS IoT connected");
  delay(2000);
}

void setup() {
  screen.initialize();

  ledCheck();

  setupWiFi();
  awsManager.setup(subscribeCallback);
  connectMQTT();

  deployerState.update("step", 0); 
  deployerState.update("button", ButtonParser::NO_BUTTON); 
  awsManager.reportState(deployerState.jsonState());

  screen.reset();
  screen.say("Hello Decisely!");
}

void loop() {
  buttonsManager.process(&buttonsActionCallback);
  awsManager.process();

  delay(50);
}