
#include <AccelStepper.h>
#include "/home/aubbiali/universita/sisEmbedded/antaDomotica/constant.h"

const int stepsPerRevolution = 4096;  // change this to fit the number of steps per revolution
const int maxTimerResponse = 2000;

// messages sent to other board
const char ds_stop = DS_STOP;
const char activate_ds_open = ACTIVATE_DS_OPEN;
const char activate_ds_close = ACTIVATE_DS_CLOSE;


// ULN2003 Motor Driver Pins
#define IN1 D1
#define IN2 D2
#define IN3 D5
#define IN4 D6

int limitSwitchOrObstacle = D7;
int buttonClose = D4;
int buttonOpen = D3;
int manAutomButton = D0;
bool rotation, isOpen, isManual;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 1500;    // the debounce time; increase if the output flickers

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
    
    readPhotoresistence();
    
  }

  if (digitalRead(manAutomButton) == LOW) {
    isManual = !isManual;
    delay(200);
  }

  delay(150);
}


const uint photoResistenceLimit = 100;
void readPhotoresistence() {
  Serial.println(analogRead(A0));
}


