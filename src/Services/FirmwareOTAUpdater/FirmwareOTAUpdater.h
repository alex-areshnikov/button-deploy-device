#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "../Wakeupper/Wakeupper.h"
#include "../../Devices/TFTScreen/TFTScreen.h"

class FirmwareOTAUpdater {
	private:
		static const int WAKEUP_INTERVAL_MS = 50;

		TFTScreen *screen;
		Wakeupper *wakeupper;

	public:
		FirmwareOTAUpdater(TFTScreen*);

		void initialize();
		void process();
};