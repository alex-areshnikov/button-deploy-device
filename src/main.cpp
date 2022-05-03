#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

#include "Devices/TFTScreen/TFTScreen.h"
#include "Devices/Buttons/ButtonsManager/ButtonsManager.h"
#include "Devices/MuxManager/MuxManager.h"
#include "Devices/Wemos/secrets.h"
#include "Devices/Wemos/WiFiConnector/WiFiConnector.h"
#include "Devices/Fingerprint/Initializer/Initializer.h"
#include "Devices/Fingerprint/Reader/Reader.h"
#include "Devices/Fingerprint/Enroller/Enroller.h"
#include "AWS/AwsManager/AwsManager.h"
#include "Services/DeployerState/DeployerState.h"
#include "Services/DocumentsProcessor/DocumentsProcessor.h"
#include "Services/DeviceActions/DeviceActions.h"
#include "Services/FirmwareOTAUpdater/FirmwareOTAUpdater.h"
#include "Services/RecurrentCounter/RecurrentCounter.h"

#include <sys/time.h> // for struct timeval
#include <time.h>

#define TFT_CS     D4
#define TFT_RST    D6                     
#define TFT_DC     D8

#define RTC_TEST 1537577487 // GMT Epoch see https://www.epochconverter.com/

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

RecurrentCounter recurrentCounter1S(1000);
RecurrentCounter recurrentCounter300MS(300);
RecurrentCounter recurrentCounter50MS(50);

// *******************************************************************************************
void setupLocalTime()
// *******************************************************************************************
{
  screen.reset();
	// see https://github.com/esp8266/Arduino/issues/4637
	time_t now; 
	now = time(nullptr); // if there's no time, this will have a value of 28800; Thu Jan  1 08:00:00 1970 
	screen.say("Initial time:"); screen.sayln(now);
	screen.sayln(ctime(&now));

	int myTimezone = -7;
	int dst = 0;
	int SecondsPerHour = 3600;
	int MAX_TIME_RETRY = 60;
	int i = 0;

	// it is unlikely that the time is already set since we have no battery; 
	// if no time is avalable, then try to set time from the network
	if (now <= 1500000000) {
		// try to set network time via ntp packets
		configTime(myTimezone * SecondsPerHour, dst * 0, "pool.ntp.org", "time.nist.gov");

		// Starting in 2007, most of the United States and Canada observe DST from
		// the second Sunday in March to the first Sunday in November.
		// example setting Pacific Time:
		setenv("TZ", "PST8PDT,M3.2.0/02:00:00,M11.1.0/02:00:00", 1); // see https://users.pja.edu.pl/~jms/qnx/help/watcom/clibref/global_data.html
		//                     | month 3, second Sunday at 2:00AM
		//                                    | Month 11 - first Sunday, at 2:00am  
		// Mm.n.d
		//   The dth day(0 <= d <= 6) of week n of month m of the year(1 <= n <= 5, 1 <= m <= 12, where 
		//     week 5 means "the last d day in month m", which may occur in the fourth or fifth week).
		//     Week 1 is the first week in which the dth day occurs.Day zero is Sunday.


		tzset();
		screen.say("Waiting for time(nullptr).");
		i = 0;
		while (!time(nullptr)) {
			screen.say(".");
			delay(1000);
			i++;
			if (i > MAX_TIME_RETRY) {
				screen.sayln("Gave up waiting for time(nullptr) to have a valid value.");
				break;
			}
		}
	}
	screen.sayln("");

	// wait and determine if we have a valid time from the network. 
	now = time(nullptr);
	i = 0;
	screen.say("Waiting for network time.");
	while (now <= 1500000000) {
		Serial.print(".");
		delay(1000); // allow a few seconds to connect to network time.
		i++;
		now = time(nullptr);
		if (i > MAX_TIME_RETRY) {
			screen.sayln("Gave up waiting for network time(nullptr) to have a valid value.");
			break;
		}
	}
	screen.sayln("");

	// get the time from the system
	screen.say("Network time:");  screen.say(now);
	screen.sayln(ctime(&now));

	// TODO - implement a web service that returns current epoch time to use when NTP unavailable (insecure SSL due to cert date validation)

	// some networks may not allow ntp protocol (e.g. guest networks) so we may need to fudge the time
	if (now <= 1500000000) {
		screen.sayln("Unable to get network time. Setting to fixed value. \n");
		// set to RTC text value
		// see https://www.systutorials.com/docs/linux/man/2-settimeofday/
		//
		//struct timeval {
		//	time_t      tv_sec;     /* seconds */
		//	suseconds_t tv_usec;    /* microseconds */
		//};
		timeval tv = { RTC_TEST, 0 };
		//
		//struct timezone {
		//	int tz_minuteswest;     /* minutes west of Greenwich */
		//	int tz_dsttime;         /* type of DST correction */
		//};
		timezone tz = { myTimezone * 60 , 0 };  

		// int settimeofday(const struct timeval *tv, const struct timezone *tz);
		settimeofday(&tv, &tz);
	}

	now = time(nullptr);
	Serial.println("Final time:");  Serial.println(now);
	Serial.println(ctime(&now));
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

void mqttConnectingCallback(char *message, int code) {
  screen.reset();
  screen.sayln("AWS IoT Error");
  screen.say("Error Code: ");
  screen.sayln(code);
  screen.say("Error Message: ");
  screen.sayln(message);
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

	awsManager.reconnect(&mqttConnectingCallback);

  screen.sayln("AWS IoT connected");
  delay(2000);
}

void setup() {
  screen.initialize();

  muxManager.ledCheck();
  muxManager.step(MuxManager::INITIALIZING_STEP);

  setupWiFi();

  setupLocalTime();
  
  awsManager.setup(subscribeCallback);
  connectMQTT();

  Fingerprint::Initializer middleFingerInitializer(&middleFinger, &screen);
  middleFingerInitializer.call();
  
  // firmwareOTAUpdater.initialize();

  deviceActions.execute(DeviceActions::READY);
}

void loop() {  
  if(recurrentCounter50MS.isReady()) {
    buttonsManager.process(&buttonsActionCallback);
    deviceActions.process();
    // firmwareOTAUpdater.process();
  }
  
  if(recurrentCounter300MS.isReady()) {
    muxManager.process();
  }

  if(recurrentCounter1S.isReady()) {
    screen.process();
    awsManager.process();

    if(muxManager.step() == MuxManager::READY_STEP) {
      middleFingerReader.process();
    }
  }
}

