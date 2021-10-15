#include <ESP8266WiFiMulti.h>

#ifndef STASSID
#define STASSID1 "Totoro_2G"
#define STAPSK1 "pinosol1"
#define STASSID2 "GD-guest"
#define STAPSK2 "Aiv6aequ"
#endif

#define NTP_PRIM "time.google.com"
#define NTP_SEC "pool.ntp.org"

// WIFI
// Configuration for NTP


class WifiConfig {
    public:
        WifiConfig();
        ~WifiConfig();
        void sleep();
        bool connect();
    private:
        const char* ssid[2] = {STASSID1, STASSID2};
        const char* password[2] = {STAPSK1, STAPSK2};
        const char* ntp_primary = NTP_PRIM;
        const char* ntp_secondary = NTP_SEC;
        ESP8266WiFiMulti wifiMulti;
};