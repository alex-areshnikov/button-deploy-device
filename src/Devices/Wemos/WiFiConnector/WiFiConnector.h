#include <ESP8266WiFi.h>

class WiFiConnector {
	private:
		const char *ssid;
		const char *pwd;

  public:
  	WiFiConnector(const char*, const char*);
		void connect(void (*connectingCallback)());
};
