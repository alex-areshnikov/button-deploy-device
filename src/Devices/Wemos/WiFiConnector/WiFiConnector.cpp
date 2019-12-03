#include "WiFiConnector.h"

WiFiConnector::WiFiConnector(const char* ssid, const char* pwd) {
  this->ssid = ssid;
	this->pwd = pwd;
}

void WiFiConnector::connect(void (*connectingCallback)()) {
	WiFi.mode(WIFI_STA);
  WiFi.begin(this->ssid, this->pwd);

  // Only try 15 times to connect to the WiFi  
  while (WiFi.status() != WL_CONNECTED){
    (*connectingCallback)();
  }
}