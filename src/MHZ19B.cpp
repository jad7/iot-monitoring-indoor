#include <MHZ19B.hpp>

//#define DEBUG 0

MHZ19B::MHZ19B(uint8_t RX, uint8_t TX) {
    serial.begin(9600, SWSERIAL_8N1,  RX, TX, false, 64, 64);
    //resetMHZ();
    //serial.onReceive([this](int a) {this->processMHZ(a);});
    #ifdef DEBUG
        Serial.println(F("MHZ construct"));
    #endif
    
}

bool MHZ19B::read(MeteoData* data) {
    if (serial.available()) {
        return processMHZ(serial.available(), data);
    }
    return false;
}


//// MHZ-19

void MHZ19B::request() {
#if defined(DEBUG)
    Serial.println(FPSTR("Sending request to MHZ"));
  #endif
  static byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
  serial.write(cmd, 9);
  if (!serial.isListening()) {
      serial.listen();
  } 
}

void MHZ19B::resetMHZ() {
  for (byte i = 0; i< 5; i++) {
    static byte resetCmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x78};
    serial.write(resetCmd, 9);
    delay(100);    
    //static byte abs_on_cmd[9] = {0xFF,0x01,0x86,0xA0,0x00,0x00,0x00,0x00,0x79};
    //mhzSerial.write(abs_on_cmd, 9);
    //delay(100);  
    static byte zero1[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x87};
    static byte zero2[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x88};
    serial.write(zero1, 9);
    delay(100);
    serial.write(zero2, 9);
    delay(1000);
  }  
}


bool MHZ19B::processMHZ(int available, MeteoData* data) {
  #if defined(DEBUG)
    Serial.print("Got response for MHZ, count bytes: ");
    Serial.println(available);
  #endif
  if (available > 8) {
    memset(co2SensorResponse, 0, 9);
    serial.readBytes(co2SensorResponse, 9);
    serial.stopListening();
    //validate 
    static byte crc;
    crc = 0;
    for (byte x = 1; x < 8; x++) {
      crc += co2SensorResponse[x];
    }
    crc = 0xff - crc;
    crc += 1;
    bool valid = (co2SensorResponse[0] == 0xFF && co2SensorResponse[1] == 0x86 && co2SensorResponse[8] == crc);
    if (valid) {
        uint16_t val = (256 * (unsigned int) co2SensorResponse[2]) + (unsigned int) co2SensorResponse[3];
        data->co2 = val;
        #if defined(DEBUG)
        Serial.print("MHZ value");
        Serial.println(val);
        #endif
    } else {
      Serial.print(FPSTR("Invalid MHZ CRC. Available:"));
      Serial.println(available);
      for (int i = 0; i < 9; i++) {
        Serial.print(i+1);
        Serial.print(": ");
        Serial.println(co2SensorResponse[i], HEX);
        Serial.print("//\\");
      }
      Serial.print("\n");
      Serial.print(FPSTR("Actual CRC: "));
      Serial.println(crc);
      Serial.print(FPSTR("Expected CRC: "));
      Serial.println(co2SensorResponse[8]);
      int val = (256 * (unsigned int) co2SensorResponse[2]) + (unsigned int) co2SensorResponse[3];
      Serial.print(FPSTR("Actual value: "));
      Serial.println(val);
      
    }    
    while(serial.available()) {serial.read();}
    return true;
  }
  return false;
}
