#include "DHT.hpp"


DHT22::DHT22(uint8_t pin){
    dht = new DHT(pin, 22);
    dht->begin();
}

void DHT22::read(MeteoData* data) {
    #ifdef DEBUG
        Serial.println(F("DHT reading data"));
      #endif
    data->humidity = (int)dht->readHumidity();
    data->tempr = dht->readTemperature();
}