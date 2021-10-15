#ifndef DHT22_H
#define DHT22_H

#include "Arduino.h"
#include "user_interface.h"
#include "MeteoStruct.h"
#include <DHT.h>
#include <DHT_U.h>

#undef DHT22
//#define DHTPIN D7     // what pin we're connected to
//#define DHTTYPE DHT22

class DHT22 {
    public:
        DHT22(uint8_t pin);
        void read(MeteoData* data);
    private:
        DHT* dht = nullptr;
};
#endif