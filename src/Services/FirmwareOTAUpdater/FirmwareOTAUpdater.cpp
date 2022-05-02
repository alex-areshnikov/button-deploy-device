#include "FirmwareOTAUpdater.h"

FirmwareOTAUpdater::FirmwareOTAUpdater(TFTScreen* screen) {	
	this->screen = screen;
}

void FirmwareOTAUpdater::initialize() {	
	screen->reset();
	screen->sayln("Initializing OTA...");

	ArduinoOTA.onStart([this]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
		screen->reset();
    screen->sayln("Start updating " + type);
  });

  ArduinoOTA.onEnd([this]() {
    screen->sayln("\nEnd");
  });

  ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
		char buffer[32];
		sprintf(buffer, "Progress: %u%%\r", (progress / (total / 100)));

    screen->say(buffer);
  });

  ArduinoOTA.onError([this](ota_error_t error) {
		char buffer[32];
		sprintf(buffer, "Error[%u]: ", error);
    screen->say(buffer);

    if (error == OTA_AUTH_ERROR) {
      screen->sayln("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      screen->sayln("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      screen->sayln("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      screen->sayln("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      screen->sayln("End Failed");
    }
  });

  ArduinoOTA.begin();
	screen->sayln("Done.");
}

void FirmwareOTAUpdater::process() {
	ArduinoOTA.handle();
}

