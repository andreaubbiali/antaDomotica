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
int buttonClose = D0;
int buttonOpen = D4;
int manAutomButton = D3;
bool rotation, isOpen, isManual, sensorResponse;

unsigned long lastDebounceTimeSwitch = 0;  // the last time the output pin was toggled
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

  // set interrupt
  pinMode(limitSwitchOrObstacle, INPUT);
  attachInterrupt(digitalPinToInterrupt(limitSwitchOrObstacle), limitSwitch, FALLING);
  pinMode(manAutomButton, INPUT);
  attachInterrupt(digitalPinToInterrupt(manAutomButton), manAutInterrupt, FALLING);
  
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
      Serial.println("BUTTON CLOSE CLICKED");
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

      moveDoorAutomaticLogic(respTime, respPhotoRes);
    }

  }

  mqttClient.loop();

  delay(150);
}

/**
* The purpose is that if I have decided to close the door it must remain closed (usually during the night) while if is automatic and I said to open it, 
* if the photresistence decide to cloe it, the door should be closed.
* 
* photoresistence -> close        time -> open   = close
* photoresistence -> open         time -> close  = close
* photoresistence -> open         time -> open   = open
* photoresistence -> close        time -> close  = close
*/
void moveDoorAutomaticLogic(String respTime, String respPhotoRes){
  if (respTime == respPhotoRes) {
    if (respTime == CLOSE){
      closeDoor();
    } else if (respTime == OPEN) {
      openDoor();
    }
  }

  if (respPhotoRes == CLOSE && respTime == OPEN){
    closeDoor();
  }

  if (respPhotoRes == OPEN && respTime == CLOSE) {
    closeDoor();
  }
}

void setIsManual(bool value){
  isManual = value;
  sendUpdateIsManual();
}


