#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

#include "Devices/TFTScreen/TFTScreen.h"
#include "Devices/Buttons/ButtonsManager/ButtonsManager.h"
#include "Devices/MuxManager/MuxManager.h"
#include "Devices/Wemos/WiFiConnector/WiFiConnector.h"
#include "Devices/Fingerprint/Initializer/Initializer.h"
#include "Devices/Fingerprint/Reader/Reader.h"
#include "Devices/Fingerprint/Enroller/Enroller.h"
#include "AWS/AwsManager/AwsManager.h"
#include "Services/DeployerState/DeployerState.h"
#include "Services/DocumentsProcessor/DocumentsProcessor.h"
#include "Services/DeviceActions/DeviceActions.h"
#include "Services/FirmwareOTAUpdater/FirmwareOTAUpdater.h"

#define WIFI_SSID "Decisely"
#define WIFI_PASSWORD "B3n3fits.2017!"

#define TFT_CS     D4
#define TFT_RST    D6                     
#define TFT_DC     D8

TFTScreen screen(TFT_CS, TFT_DC, TFT_RST);
Adafruit_Fingerprint middleFinger = Adafruit_Fingerprint(&Serial);
ButtonsManager buttonsManager;
MuxManager muxManager(D0, D1, D2, D3);
WiFiConnector wiFiConnector(WIFI_SSID, WIFI_PASSWORD);
DeployerState deployerState;
AwsManager awsManager;
DeviceActions deviceActions(&screen, &muxManager, &awsManager, &deployerState);
FirmwareOTAUpdater firmwareOTAUpdater(&screen);

StaticJsonDocument<1024> awsIotJson;

void debugCallback(char* message) {
  screen.reset();
  screen.say(message);
  delay(500);
}

void connectingCallback() {
  screen.say(".");
  delay(500);
}

void scanSuccessCallback(int fingerID) {
  deployerState.update("finger_id", fingerID);
  deviceActions.execute(DeviceActions::ACCESS_GRANTED);
  deviceActions.execute(DeviceActions::READY, 15);  
}

void scanFailureCallback() {
  deviceActions.execute(DeviceActions::ACCESS_DENIED);
  deviceActions.execute(DeviceActions::READY, 3);
}
Fingerprint::Reader middleFingerReader(&middleFinger, &screen, &scanSuccessCallback, &scanFailureCallback);

void subscribeCallback(char* topic, byte* payload, unsigned int length) {
  char trimmedPayload[length];
  strlcpy(trimmedPayload, (char*)payload, length+1);

  DeserializationError deserializationError = deserializeJson(awsIotJson, trimmedPayload);

  if (deserializationError) {
    screen.reset();
    screen.sayln("Incoming message on topic: ");
    screen.sayln(topic);
    screen.sayln("\nDeserialization Error:");
    screen.sayln(deserializationError.c_str());
  } else if(strcmp(topic, AWS_IOT_UPDATE_DOCUMENTS_TOPIC) == 0) { 
    DocumentsProcessor documentsProcessor(&awsIotJson);
    // FingerprintDocumentsProcessor fingerprintDocumentsProcessor(&awsIotJson);
    documentsProcessor.process();

    if(documentsProcessor.hasValidStep()) {
      muxManager.step(documentsProcessor.step(), documentsProcessor.isErred());
      deployerState.update("step", documentsProcessor.step());        
      deployerState.update("error", documentsProcessor.isErred());

      if(documentsProcessor.isErred()) {
        deviceActions.execute(DeviceActions::READY, 5);
      }
    } 

    if(documentsProcessor.isFingerprintEnroll()) {
      Fingerprint::Enroller middleFingerEnroller(&middleFinger, &screen, documentsProcessor.fingerprintEnrollId);

      muxManager.step(MuxManager::FINGERPRINT_SCAN_STEP);
      deployerState.update("step", MuxManager::FINGERPRINT_SCAN_STEP);
      deployerState.update("enrolling", true);
      awsManager.reportState(deployerState.jsonState());

      if(middleFingerEnroller.call() == Fingerprint::Enroller::ENROLL_OK) {
        deployerState.update("enrolled", true);
        deployerState.update("finger_id", documentsProcessor.fingerprintEnrollId);
      }

      deployerState.update("enrolling", false);
      deviceActions.execute(DeviceActions::READY, 5);
    }

    if(documentsProcessor.hasDesiredKey()) {
      awsManager.reportState(deployerState.jsonState());     
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

  muxManager.ledCheck();
  muxManager.step(MuxManager::INITIALIZING_STEP);

  setupWiFi();
  
  awsManager.setup(subscribeCallback);
  connectMQTT();

  Fingerprint::Initializer middleFingerInitializer(&middleFinger, &screen);
  middleFingerInitializer.call();
  
  // firmwareOTAUpdater.initialize();

  deviceActions.execute(DeviceActions::READY);
}

void loop() {
  buttonsManager.process(&buttonsActionCallback);
  awsManager.process();
  muxManager.process();
  deviceActions.process();
  screen.process();
  // firmwareOTAUpdater.process();

  if(muxManager.step() == MuxManager::READY_STEP) {
    middleFingerReader.process();
  }

  delay(50);
}

