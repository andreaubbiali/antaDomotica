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
* Open door if not already opened and the other board is able to start the distance sensor.
*/
void openDoor() {
  Serial.println("OPEN CLICK");

  if (!isOpen && comunicateMovement(activate_ds_open)) {

    stepper.move(-stepsPerRevolution); 

    rotation = true;
    lastDebounceTime = millis();
    moveDoor();
    isOpen = true;
  }
}

/**
* Close door if not already closed and the other board is able to start the distance sensor.
*/
void closeDoor() {
  Serial.println("CLOSE CLICK");

  if (isOpen && comunicateMovement(activate_ds_close)){
    stepper.move(stepsPerRevolution); 

    rotation = true;
    lastDebounceTime = millis();
    moveDoor();
    isOpen = false;
  }
  
}

/**
* The door is going to move, we have to start the distance sensor on the other board and wait for the response to be sure it has been activated.
*/
bool comunicateMovement(char movementCode){

  // comunicate to the other board
  Serial.print(movementCode);
  bool response = false;
  int timerResponse = millis();

  while(!response && ((millis() - timerResponse) < maxTimerResponse) ) {

    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c == DS_ACTIVATED) {
        response = true;
      }
    }

  }
  
  return response;
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
* No obstacle sensor is activated so for this first passage be sure the radius of movement is free.
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


