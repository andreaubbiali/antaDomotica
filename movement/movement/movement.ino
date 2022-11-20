
#include <AccelStepper.h>
#include "EspMQTTClient.h" //https://github.com/plapointe6/EspMQTTClient
#include "/home/aubbiali/universita/sisEmbedded/WifiConfig.h"
#include "/home/aubbiali/universita/sisEmbedded/antaDomotica/constant.h"

const int stepsPerRevolution = 4096;  // change this to fit the number of steps per revolution
const int maxTimerResponse = 8000;

// TODO set setKeepAlive
EspMQTTClient mqttClient(
  WIFI_SSID,
  WIFI_PSW,
  "test.mosquitto.org",
  "movementClient",
  1883
);


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

uint countPhotoResistenceUp;
uint countPhotoResistenceLow;
const uint readPhotoResistenceInterval = 10000;
uint lastPhotoresistenceRead = millis();

// AccelStepper::HALF4WIRE -> to indicate we’re controlling the stepper motor with four wires
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

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

  initialDoorSetup();
  isManual = true;
}

void loop() {

  if (isManual) {
    if (digitalRead(buttonClose) == LOW) {
      closeDoor();
    } else if (digitalRead(buttonOpen) == LOW) {
      openDoor();
    }
  } else {
    
    if (millis() - lastPhotoresistenceRead > readPhotoResistenceInterval) {
      // read photoresistence
      lastPhotoresistenceRead = millis();
      readPhotoresistence();
    }
  }

  if (digitalRead(manAutomButton) == LOW) {
    isManual = !isManual;
    countPhotoResistenceUp = 0;
    countPhotoResistenceLow = 0;
    delay(100);
  }

  mqttClient.loop();
  if (!mqttClient.isConnected()) {
    Serial.println("not connected");
  }

  delay(150);
}


