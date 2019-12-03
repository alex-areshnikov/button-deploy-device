#include <Arduino.h>
#include "Devices/TFTScreen/TFTScreen.h"
#include "Devices/Buttons/ButtonsManager/ButtonsManager.h"
#include "Devices/MuxManager/MuxManager.h"
#include "Devices/Wemos/WiFiConnector/WiFiConnector.h"
#include "AWS/AwsManager/AwsManager.h"
#include "Services/DeployerState/DeployerState.h"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define TFT_CS     D4
#define TFT_RST    D6                     
#define TFT_DC     D8

TFTScreen screen(TFT_CS, TFT_DC, TFT_RST);
ButtonsManager buttonsManager;
MuxManager muxManager(D0, D1, D2, D3);
WiFiConnector wiFiConnector(WIFI_SSID, WIFI_PASSWORD);
AwsManager awsManager;
DeployerState deployerState;

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
  screen.say(String(message));
  delay(500);
}

void connectingCallback() {
  screen.say(String("."));
  delay(500);
}

void buttonsActionCallback(const char* buttonCode) {
  deployerState.update("button", buttonCode);
  awsManager.reportState(deployerState.jsonState());

  // screen.reset();
  // screen.say(String(buttonCode));

  // if(buttonsManager.isDownButton(buttonCode)) {
  //   muxManager.prevStep();
  // }

  // if(buttonsManager.isUpButton(buttonCode)) {
  //   muxManager.nextStep();
  // }

  // if(buttonsManager.isSelectButton(buttonCode)) {
  //   muxManager.readyStep();
  //   ledCheck();
  // }
}

void setupWiFi() {
  screen.say(String("Connecting to WiFi"));
  wiFiConnector.connect(&connectingCallback);

  screen.reset();
  screen.sayln(String("WiFi connected"));
  screen.say(String("IP address: "));
  screen.sayln(WiFi.localIP().toString());
  screen.say(String("RSSI: "));
  screen.sayln(String(WiFi.RSSI()));
  delay(2000);
}

void setupMQTT() {
  screen.reset();
  screen.say(String("Connecting to AWS IoT"));

  if(awsManager.connect(&connectingCallback)) {
    screen.reset();
    screen.sayln(String("AWS IoT connected"));
    delay(2000);
  } else {
    screen.sayln(String("FAILED"));
    delay(2000);
    ESP.reset();
  }
}

void setup() {
  screen.initialize();

  ledCheck();

  setupWiFi();
  setupMQTT();

  screen.reset();
  screen.say(String("Hello Decisely!"));
}

void loop() {
  buttonsManager.process(&buttonsActionCallback);
  awsManager.process();
}