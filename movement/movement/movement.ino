#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <AccelStepper.h>
#include "EspMQTTClient.h" //https://github.com/plapointe6/EspMQTTClient
#include "/home/aubbiali/universita/sisEmbedded/WifiConfig.h"
#include "/home/aubbiali/universita/sisEmbedded/antaDomotica/constant.h"

const int stepsPerRevolution = 4096;  // change this to fit the number of steps per revolution
const int maxTimerResponse = 8000;
const int automaticReadMinutes = AUTOMATIC_READ_MINUTE;
const String CLOSE = "close";
const String OPEN = "open";

// ULN2003 Motor Driver Pins
#define IN1 D1
#define IN2 D2
#define IN3 D5
#define IN4 D6

int limitSwitchOrObstacle = D7;
int buttonClose = D4;
int buttonOpen = D3;
int manAutomButton = D0;
bool rotation, isOpen, isManual, sensorResponse;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 1500;    // the debounce time; increase if the output flickers

uint lastAutomaticaRead = millis();

// AccelStepper::HALF4WIRE -> to indicate we’re controlling the stepper motor with four wires
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

// MQTT CLIENT
EspMQTTClient mqttClient(
  WIFI_SSID,
  WIFI_PSW,
  "test.mosquitto.org",
  "movementClient",
  1883
);

// NTP CLIENT to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup() {
  Serial.begin(115200);
  
  // set stepper's speed and acceleration
  resetStepper();

  pinMode(buttonClose, INPUT);
  pinMode(buttonOpen, INPUT);
  pinMode(manAutomButton, INPUT);

  // set interrupt
  pinMode(limitSwitchOrObstacle, INPUT);
  attachInterrupt(digitalPinToInterrupt(limitSwitchOrObstacle), limitSwitch, FALLING);
  
  while(!mqttClient.isConnected()){
    mqttClient.loop();
    delay(500);
  }

  setupNTP();

  Serial.println("CARICATO TUTTO");  

  initialDoorSetup();
  setIsManual(true);
}

void loop() {

  if (isManual) {
    if (digitalRead(buttonClose) == LOW) {
      closeDoor();
    } else if (digitalRead(buttonOpen) == LOW) {
      openDoor();
    }
  } else {
    
    if (millis() - lastAutomaticaRead > (automaticReadMinutes*60000)) {
      lastAutomaticaRead = millis();
      
      // get the movement to do based on time
      String respTime = movementOnTime();

      // read photoresistence
      String respPhotoRes = readPhotoresistence();

      if (respTime == respPhotoRes) {
        if (respTime == CLOSE){
          closeDoor();
        } else if (respTime == OPEN) {
          openDoor();
        }
      }
    }

  }

  if (digitalRead(manAutomButton) == LOW) {
    isManual = !isManual;
    delay(100);
  }

  mqttClient.loop();

  delay(150);
}

void setIsManual(bool value){
  isManual = value;
  sendUpdateIsManual();
}


