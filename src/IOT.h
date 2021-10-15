#ifndef IOT_H_
#define IOT_H_
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <MeteoStruct.h>
#include "WiFiClientSecureBearSSL.h"
#include <time.h>

#include <MQTT.h>

#include <CloudIoTCore.h>
#include <CloudIoTCoreMqtt.h>

class IOT {
    public:
        IOT(const char* project_id, const char* location, 
        const char* registry_id, const char* device_id,
        unsigned const char* private_key, const char* primary_ca, const char* backup_ca);
        ~IOT();
        bool connect();
        bool sendData(MeteoData* data);
        String getJwtImt();
        void print_data(MeteoData* data);
        BearSSL::WiFiClientSecure netClient;
        
    private:
        StaticJsonDocument<256> doc;
        bool settedUp = false;
        MQTTClient *mqttClient;
        
        BearSSL::X509List certList;
        CloudIoTCoreDevice *device;
        CloudIoTCoreMqtt *mqtt;
        unsigned const char* private_key;
        const char* primary_ca;
        const char* backup_ca;

        void setupCertAndPrivateKey();
        void setup();
        void publish_data();
        
        bool publishTelemetry(String data);
        bool publishTelemetry(const char *data, int length);
        bool publishTelemetry(String subfolder, String data);
        bool publishTelemetry(String subfolder, const char *data, int length);
};

#endif