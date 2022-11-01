/**
* Set the door to the open state.
* No obstacle are checked in this moment.
*/
void initialDoorSetup() {
  
  // limit switch is clicked
  if (digitalRead(limitSwitchOrObstacle) == LOW) {
    tryOpenDoor();
  } else {
    isOpen = false;
    openDoor();
  }

}

/**
* Open door.
*/
void openDoor() {
  Serial.println("OPEN CLICK");

  if (!isOpen) {
    stepper.move(-stepsPerRevolution); 

    rotation = true;
    lastDebounceTime = millis();
    moveDoor();
    isOpen = true;
  }
}

/**
* Close door.
*/
void closeDoor() {
  Serial.println("CLOSE CLICK");

  if (isOpen){
    stepper.move(stepsPerRevolution); 

    rotation = true;
    lastDebounceTime = millis();
    moveDoor();
    isOpen = false;
  }
  
}

/**
* Move door.
*/
void moveDoor() {

  while(rotation) {
    stepper.run();
    yield();
  }

}

/**
* Try to put the door in the open state.
* Now the door is at one of the limit switch but we don't know which of them (the open or close one).
*/
void tryOpenDoor() {

  rotation = true;
  stepper.move(-stepsPerRevolution);
  lastDebounceTime = millis();

  while(rotation) {
    
    // check if we are already in the open state.
    if (digitalRead(limitSwitchOrObstacle) == LOW && ((millis() - lastDebounceTime) > debounceDelay)) {
      rotation = false;
    }

    stepper.run();
    yield();

  }

  resetStepper();
  isOpen = true;
}

void resetStepper(){
  stepper.setCurrentPosition(0);
  stepper.setMaxSpeed(700);
  stepper.setAcceleration(200);
}


