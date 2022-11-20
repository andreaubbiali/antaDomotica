#include <DHT.h>
#include "EspMQTTClient.h" //https://github.com/plapointe6/EspMQTTClient
#include "/home/aubbiali/universita/sisEmbedded/WifiConfig.h"
#include "/home/aubbiali/universita/sisEmbedded/antaDomotica/constant.h"

#define DHTPIN D7  // Digital pin connected to the DHT sensor
#define trigPinClose D2
#define echoPinClose D5

#define DHTTYPE DHT11

const int obstaclePinSignal = D0;
float temperature = 1.0;
float duration, distance;
bool checkSensor;
int trigP;
int echoP;

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
  pinMode(obstaclePinSignal, OUTPUT);

  checkSensor = false;

  digitalWrite(obstaclePinSignal, HIGH);
  dht.begin();
}

void loop() {

  if (checkSensor) {
    distanceSensor();
  }
    
  mqttClient.loop();
  delay(150);    
  
  // if (Serial.available() > 0) {
  //   char msg = Serial.read();
  //   if (msg == activate_ds_close) {
  //     Serial.println("CLOSE RECEIVED");
  //     distanceSensor(trigPinClose, echoPinClose);
  //   } else if (msg == activate_ds_open) {
  //     Serial.println("OPEN RECEIVED");
  //     distanceSensor(trigPinClose, echoPinClose);
  //   }
  //   // if (c == ACTIVATE_DS_LEFT) {
  //   //   
  //   // } else if (c == ACTIVATE_DS_RIGHT){
  //   //   // distanceSensor(trigPin, echoPin);
  //   // }
  // }
}

