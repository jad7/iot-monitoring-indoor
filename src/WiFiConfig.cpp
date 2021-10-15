#include <WiFiConfig.h>

#include <time.h>
//#include <ESP8266WiFi.h>
#include "Arduino.h"

#define CONNECTION_TIMEOUT_MS 5000


WifiConfig::WifiConfig() {
  for (unsigned int i=0; i < sizeof(ssid)/sizeof(ssid[0]); i++) {
    wifiMulti.addAP(ssid[i], password[i]);
  }
}

WifiConfig::~WifiConfig() {
}

void WifiConfig::sleep() {
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
}

bool WifiConfig::connect() {
  #ifdef DEBUG
    Serial.println(F("Connecting to WiFi"));
  #endif
  WiFi.forceSleepWake();
  delay(10);
  WiFi.mode(WIFI_STA);
  if (wifiMulti.run(CONNECTION_TIMEOUT_MS) == WL_CONNECTED) {
    WiFi.setSleepMode(WIFI_MODEM_SLEEP, 0);
    delay(1);
    Serial.println(F("Time config"));  

    configTime(0, 0, ntp_primary, ntp_secondary);
    Serial.println(F("Waiting on time sync..."));
    while (time(nullptr) < 1510644967) {
      delay(10);
    }
    Serial.println(F("Time config DONE!"));
    return true;
  } else {
    return false;
  }
}
