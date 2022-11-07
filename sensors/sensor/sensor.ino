#include <DHT.h>
#include "/home/aubbiali/universita/sisEmbedded/antaDomotica/constant.h"

#define DHTPIN D7  // Digital pin connected to the DHT sensor
#define trigPinClose D2
#define echoPinClose D5

#define DHTTYPE DHT11

const char activate_ds_close = ACTIVATE_DS_CLOSE;
const char activate_ds_open = ACTIVATE_DS_OPEN;
const char ds_activated = DS_ACTIVATED;
const char ds_stop = DS_STOP;

DHT dht(DHTPIN, DHTTYPE);

float temperature = 1.0;
float duration, distance;
const int obstaclePinSignal = D0;

void setup() {
  Serial.begin(115200);
  pinMode(trigPinClose, OUTPUT);
  pinMode(echoPinClose, INPUT);
  pinMode(obstaclePinSignal, OUTPUT);

  digitalWrite(obstaclePinSignal, HIGH);
  dht.begin();
}

void loop() {
  
  if (Serial.available() > 0) {
    char msg = Serial.read();
    if (msg == activate_ds_close) {
      Serial.println("CLOSE RECEIVED");
      distanceSensor(trigPinClose, echoPinClose);
    } else if (msg == activate_ds_open) {
      Serial.println("OPEN RECEIVED");
      distanceSensor(trigPinClose, echoPinClose);
    }
    // if (c == ACTIVATE_DS_LEFT) {
    //   
    // } else if (c == ACTIVATE_DS_RIGHT){
    //   // distanceSensor(trigPin, echoPin);
    // }
  }
}

