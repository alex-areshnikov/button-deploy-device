#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "../../Devices/TFTScreen/TFTScreen.h"

class FirmwareOTAUpdater {
	private:
		TFTScreen *screen;
	public:
		FirmwareOTAUpdater(TFTScreen*);

		void initialize();
		void process();
};