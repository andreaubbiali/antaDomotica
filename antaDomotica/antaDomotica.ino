
#include <AccelStepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
const int closeLimitValue = 100;
const int openLimitValue = 200;

// ULN2003 Motor Driver Pins
#define IN1 5  //D1
#define IN2 4  //D2
#define IN3 14 //D5
#define IN4 12 //D6

int swithInput = D7;
int startRotationInput = D6;
bool rotation = false;
int countClose = 0;
int countOpen = 0;
// isOPEN indicate if the door is open or not. isPRActive indicate if the photoresistence is active.
bool isOPEN, isPRActive;

// AccelStepper::HALF4WIRE -> to indicate we’re controlling the stepper motor with four wires
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

void setup() {
  // initialize the serial port
  Serial.begin(9600);
  
  // set the speed and acceleration
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(100);
  // set target position  
  stepper.runToPosition(0);
  isOPEN = false;
  isPRActive = true;

  pinMode(swithInput, INPUT);
  pinMode(startRotationInput, INPUT);

  attachInterrupt(digitalPinToInterrupt(swithInput), limitSwitch, FALLING);  
  attachInterrupt(digitalPinToInterrupt(startRotationInput), startRotation, FALLING);
}

void loop() {

  if (isPRActive) {
    photoResistenceRead();    
  }
  
  delay(100);

}

/*
* Read photoresistence and if is a certain value start close/open
*/
void photoResistenceRead() {
  int value = analogRead(A0);

  if (isOpen) {
    if (value > closeLimitValue) {
      countClose += 1;
      if (countClose >= ) {
        // close
      }
    } 
  } else {

    if (value < openLimitValue) {
      // open
    }    
  }

}


ICACHE_RAM_ATTR void limitSwitch() {
  Serial.println("FINE CORSA CLICCATO");

  rotation = false;
}

ICACHE_RAM_ATTR void startRotation() {
  Serial.println("ENTRA start rotation");

  rotation = true;
}


