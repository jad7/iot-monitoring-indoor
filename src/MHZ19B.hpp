#ifndef BME_H
#define BME_H
#include <MeteoStruct.h>
#include "SoftwareSerial.h"

class MHZ19B {
    public:
        MHZ19B(uint8_t RX, uint8_t TX);
        void request();
        bool read(MeteoData* data);
        void setup();
    private:
        void resetMHZ();
        bool processMHZ(int available, MeteoData* data);
        SoftwareSerial serial;
        unsigned char co2SensorResponse[9];
        Delegate<void(uint16_t co2), void*> handler;
};
#endif