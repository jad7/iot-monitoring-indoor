#include <IOT.h>
#include <jwt.h>

#include "FS.h"
#define DEBUG 1
#define jwt_exp_secs 3600
IOT* iot_singletone;

//Constructor
IOT::IOT(const char* project_id, const char* location, 
        const char* registry_id, const char* device_id,
        unsigned const char private_key[], const char* primary_ca, const char* backup_ca) {
    device = new CloudIoTCoreDevice(project_id, location, registry_id, device_id);
    this->primary_ca = primary_ca;
    this->private_key = private_key;
    this->backup_ca = backup_ca;
    iot_singletone = this;
}

IOT::~IOT() {
    delete device;
    if (mqtt) {
        delete mqtt;
    }
}

bool IOT::connect() {
    if (!this->settedUp)
    {
        this->setup();
        this->settedUp = true;

    }
#if defined(DEBUG)
  Serial.println("Start checking connection");
#endif 

  if (!mqtt->loop()) {
    #if defined(DEBUG)
    Serial.println("Mqtt connection....");
    #endif 
    mqtt->mqttConnect();
    //TODO retry
  }
  #if defined(DEBUG)
    Serial.println("Mqtt connected");
  #endif 
  return true;
}


bool IOT::sendData(MeteoData* data) {
    #if defined(DEBUG)
  Serial.println(F("Start publish data"));
  #endif
  print_data(data);
  doc.clear();
  if (data->tempr > -50.0) {
    doc[FPSTR("temp")] = data->tempr;
    doc[FPSTR("hum")] = data->humidity;
  }
  if (data->co2>0) {
    doc[FPSTR("co2")] = data->co2;
  }
  String output; //= "{\"co2\": 400}";
  serializeJson(doc, output);
  #if defined(DEBUG)
  Serial.print(F("Output:"));
  Serial.println(output);
  #endif
  if (this->publishTelemetry(output)) {
    return true;
  } else {
    mqtt->logError();
    return false;
  }
  
}

void IOT::print_data(MeteoData* data)
{
  Serial.print(F("T: "));
  Serial.print(data->tempr);
  Serial.print(F(", H: "));
  Serial.print(data->humidity);
  Serial.print(F(", CO2: "));
  Serial.print(data->co2);
}

//private
void IOT::setup()
{
  // ESP8266 WiFi secure initialization and device private key
  setupCertAndPrivateKey();

  mqttClient = new MQTTClient(512);
  mqttClient->setOptions(5, true, 2000); // keepAlive, cleanSession, timeout
  mqtt = new CloudIoTCoreMqtt(mqttClient, &netClient, device);
  mqtt->setUseLts(true);
  mqtt->startMQTTAdvanced(); // Opens connection using advanced callback
}


String IOT::getJwtImt()
{
  // Disable software watchdog as these operations can take a while.
  ESP.wdtDisable();
  time_t iat = time(nullptr);
  Serial.println("Refreshing JWT");
  String jwt = device->createJWT(iat, jwt_exp_secs);
  ESP.wdtEnable(0);
  return jwt;
}


void IOT::setupCertAndPrivateKey()
{
  // Set CA cert on wifi client
  // If using a static (pem) cert, uncomment in ciotc_config.h:
  certList.append(primary_ca);
  certList.append(backup_ca);
  netClient.setTrustAnchors(&certList);

  device->setPrivateKey(private_key);
  return;

}


bool IOT::publishTelemetry(String data)
{
  return mqtt->publishTelemetry(data);
}

bool IOT::publishTelemetry(const char *data, int length)
{
  return mqtt->publishTelemetry(data, length);
}

bool IOT::publishTelemetry(String subfolder, String data)
{
  return mqtt->publishTelemetry(subfolder, data);
}

bool IOT::publishTelemetry(String subfolder, const char *data, int length)
{
  return mqtt->publishTelemetry(subfolder, data, length);
}



String getJwt() {
    return iot_singletone->getJwtImt();
}

void messageReceivedAdvanced(MQTTClient *client, char topic[], char bytes[], int length)
{
  if (length > 0){
    Serial.printf("incoming: %s - %s\n", topic, bytes);
  } else {
    Serial.printf("0\n"); // Success but no message
  }
}