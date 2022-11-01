#include <DHT.h>
// #include "EspMQTTClient.h"
// #include "/home/aubbiali/Arduino/WifiConfig.h"
//https://github.com/plapointe6/EspMQTTClient
#include "/home/aubbiali/Arduino/antaDomotica/constant.h"

#define DHTPIN D7  // Digital pin connected to the DHT sensor
#define trigPinL D2
#define echoPinL D5

#define DHTTYPE DHT11

// EspMQTTClient client(
//   WIFI_SSID,
//   WIFI_PSW,
//   "test.mosquitto.org",  // MQTT Broker server ip
//   "TestClient",      // Client name that uniquely identify your device
//   1883
// );

DHT dht(DHTPIN, DHTTYPE);

float temperature = 1.0;
float duration, distance;
const int obstaclePinSignal = D0;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(obstaclePinSignal, OUTPUT);
  digitalWrite(obstaclePinSignal, HIGH);
  dht.begin();
}

void loop() {
  
  if (Serial.available() > 0) {
    int c = Serial.read();
    if (c == ACTIVATE_DS_LEFT) {
      distanceSensor(trigPinL, echoPinL);
    } else if (c == ACTIVATE_DS_RIGHT){
      distanceSensor(trigPin, echoPin);
    }
  }

  

  // if (!client.isConnected()){
  //   Serial.println("NOT CONNECTED");
  // }

  // if (!client.isWifiConnected()){
  //   Serial.println("WIFI NOT CONNECTED");
  // }

  // if (!client.isMqttConnected()) {
  //   Serial.println("MQTT NOT CONNECTED");
  // }

  // client.loop();
}

