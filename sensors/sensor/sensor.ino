#include <DHT.h>
#include "EspMQTTClient.h"
#include "/home/aubbiali/universita/sisEmbedded/WifiConfig.h"
#include "/home/aubbiali/universita/sisEmbedded/antaDomotica/constant.h"

#define DHTPIN D7  // Digital pin connected to the DHT sensor
#define trigPinClose D2
#define echoPinClose D5

#define trigPinOpen D6
#define echoPinOpen D1

#define DHTTYPE DHT11

const int obstaclePinSignal = D0;
float temperature = 1.0;
float duration, distance;
bool checkSensor;
uint trigP, echoP;

DHT dht(DHTPIN, DHTTYPE);

EspMQTTClient mqttClient(
  WIFI_SSID,
  WIFI_PSW,
  "test.mosquitto.org",
  "distanceSensorClient",
  1883
);

void setup() {
  Serial.begin(115200);
  pinMode(trigPinClose, OUTPUT);
  pinMode(echoPinClose, INPUT);
  pinMode(trigPinOpen, OUTPUT);
  pinMode(echoPinOpen, INPUT);
  pinMode(obstaclePinSignal, OUTPUT); 

  checkSensor = false;

  digitalWrite(obstaclePinSignal, HIGH);
  dht.begin();

  while(!mqttClient.isConnected()){
    mqttClient.loop();
    delay(100);
  }
  
  readTemperature();
}

void loop() {

  if (checkSensor) {
    distanceSensor();
  }
    
  mqttClient.loop();
  delay(150);    
}

