
#include <AccelStepper.h>

const int stepsPerRevolution = 4096;  // change this to fit the number of steps per revolution
// const int closeLimitValue = 100;
// const int openLimitValue = 200;

// ULN2003 Motor Driver Pins
#define IN1 D1
#define IN2 D2
#define IN3 D5
#define IN4 D6

int swithInput = D7;
int startRotationInput = D3;
bool rotation, isLeftRotation;
// int countClose = 0;
// int countOpen = 0;
// // isOPEN indicate if the door is open or not. isPRActive indicate if the photoresistence is active.
// bool isOPEN, isPRActive;

// AccelStepper::HALF4WIRE -> to indicate we’re controlling the stepper motor with four wires
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(115200);
  
  rotation = true;

  // set the speed and acceleration
  stepper.setMaxSpeed(600);
  stepper.setAcceleration(150);
  
  // set target position  
  stepper.runToNewPosition(stepsPerRevolution);
  isLeftRotation = false;
  // isOPEN = false;
  // isPRActive = true;

  pinMode(swithInput, INPUT_PULLUP);
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

  // if (isPRActive) {
  //   photoResistenceRead();    
  // }
  
  // delay(100);

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
  if (rotation) {
    Serial.println("FINE CORSA CLICCATO");
    
    isLeftRotation = !isLeftRotation;
    stepper.setCurrentPosition(0);
    stepper.setMaxSpeed(600);
    stepper.setAcceleration(150);

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
}


