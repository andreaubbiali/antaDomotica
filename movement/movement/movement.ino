
#include <AccelStepper.h>

const int stepsPerRevolution = 4096;  // change this to fit the number of steps per revolution

// ULN2003 Motor Driver Pins
#define IN1 D1
#define IN2 D2
#define IN3 D5
#define IN4 D6

int swithInput = D7;
int startRotationInput = D3;
bool rotation, isLeftRotation;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 1500;    // the debounce time; increase if the output flickers

// AccelStepper::HALF4WIRE -> to indicate we’re controlling the stepper motor with four wires
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(115200);
  
  rotation = false;

  // set the speed and acceleration
  stepper.setMaxSpeed(700);
  stepper.setAcceleration(200);

  isLeftRotation = false;

  pinMode(swithInput, INPUT);
  pinMode(startRotationInput, INPUT);

  attachInterrupt(digitalPinToInterrupt(swithInput), limitSwitch, FALLING);  
  attachInterrupt(digitalPinToInterrupt(startRotationInput), startRotation, FALLING);
}

void loop() {

  if (rotation) {
    if (stepper.distanceToGo() == 0) {
      stepper.move(getDirection() * stepsPerRevolution);      
    }

    // move the stepper motor (one step at a time)
    stepper.run();
  }

}

/*
* Read photoresistence and if is a certain value start close/open
*/
void photoResistenceRead() {
  // int value = analogRead(A0);

  // if (isOpen) {
  //   if (value > closeLimitValue) {
  //     countClose += 1;
  //     if (countClose >= ) {
  //       // close
  //     }
  //   } 
  // } else {

  //   if (value < openLimitValue) {
  //     // open
  //   }    
  // }

}


ICACHE_RAM_ATTR void limitSwitch() {

  if (rotation && ((millis() - lastDebounceTime) > debounceDelay)) {
    Serial.println("FINE CORSA CLICCATO");
    
    isLeftRotation = !isLeftRotation;
    stepper.setCurrentPosition(0);
    stepper.setMaxSpeed(700);
    stepper.setAcceleration(200);

    rotation = false;
  }
}

int getDirection() {
  if (isLeftRotation) {
    return -1;
  } else {
    return 1;
  }
}

ICACHE_RAM_ATTR void startRotation() {
  Serial.println("ENTRA start rotation");
  
  rotation = true;
  lastDebounceTime = millis();
}


