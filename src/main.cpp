//#define DEBUG

#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#define _TASK_STD_FUNCTION
#include <TaskScheduler.h>
#include "Esp.h"
#include "user_interface.h"

#include "WiFiConfig.h"
#include <MeteoStruct.h>
#include <MHZ19B.hpp>
#include <DHT.hpp>
#include <IOT.h>
#include <Keys.hpp>
#include <Display.hpp>


#define SLEEP_INTERVAL 1 * 60 * 1e6 //1min
#define WARMUP_INTERVAL 3 * 1e6 //3 sec 

Scheduler scheduler;

WifiConfig wifi;
MHZ19B mhz(D5, D6);
DHT22 dht(D7);
StatusRequest measureDone;
Display display;



IOT iot(project_id, location_id, regstry_id, device_id, (unsigned char*)private_key_v, primary_ca_v, backup_ca_v);

MeteoData current_state = {-100.0, -1, 0};



void print_data();
void storeData();

/*
Task bmeMesure(60 * TASK_SECOND, TASK_ONCE, &bmeReadValue, &scheduler, false, NULL, []()
               { measureDone.signal(1); });
Task pmsMesure(1 * TASK_SECOND, TASK_ONCE, &pmsReadValue, &scheduler, false, NULL, []()
               { pms.sleep(); measureDone.signal(1); });
Task dhtMesure(30 * TASK_SECOND, TASK_ONCE, []() { 
  dht.read(&current_state); 
  scheduler.currentTask().disable();
}, &scheduler, false, NULL, []() { measureDone.signal(1); });
*/
//Task storeDataTask(1 * TASK_SECOND, TASK_FOREVER, &storeData, &scheduler, true, NULL, &deepSleep);

Task dhtMesure(20 * TASK_SECOND, TASK_FOREVER, []() { 
  dht.read(&current_state); 
}, &scheduler, true, NULL, NULL);

Task co2WaitResponse(TASK_SECOND, TASK_FOREVER, []() { 
  if (mhz.read(&current_state)) {
    scheduler.currentTask().disable();
  }
}, &scheduler, false, NULL, NULL);

Task co2Request(30 * TASK_SECOND, TASK_FOREVER, []() { 
  mhz.request(); 
  co2WaitResponse.enable();
}, &scheduler, true, NULL, NULL);


Task showDisplay(10 * TASK_SECOND, TASK_FOREVER, []() { 
  display.print(&current_state);
}, &scheduler, true, NULL, NULL);

Task printData(10 * TASK_SECOND, TASK_FOREVER, []() { 
  print_data();
}, &scheduler, false, NULL, NULL);

Task storeDataTask(10 * TASK_MINUTE, TASK_FOREVER, &storeData, &scheduler, false);


void storeData() {
  #ifdef DEBUG
    Serial.println(F("Preparing for storing data"));
  #endif
  iot.print_data(&current_state);
  int retry = 10;
  bool success = false;
  while (!success && retry-- > 0) {
    if (wifi.connect() && iot.connect()) {
      success = true;
      iot.sendData(&current_state);
      wifi.sleep();
    }
    char buf[256];
    auto code = iot.netClient.getLastSSLError(buf, 256);
    if (code != 0) {
        Serial.print(F("Last SSL ERROR was: "));
        Serial.print(code);
        Serial.print(F(" - "));
        Serial.println(buf);
    }
  }
  digitalWrite(LED_BUILTIN, success ? LOW : HIGH);
  storeDataTask.disable();
}

uint32_t deepSleepState = 0;

void setup() {

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  #ifdef DEBUG
    Serial.println(F("Hello!"));
    delay(10);
  #endif
  display.setup();
  wifi.connect();
  storeDataTask.enableDelayed(20 * TASK_SECOND);
  
  //wifi.sleep();
  
}



void loop() {
  scheduler.execute();
}

void print_data()
{
  Serial.print(F("T: "));
  Serial.print(current_state.tempr);
  Serial.print(F(", H: "));
  Serial.print(current_state.humidity);
  Serial.print(F(", CO2: "));
  Serial.print(current_state.co2);
  Serial.println();
}
