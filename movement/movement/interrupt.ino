/**
* Interrupt when a limit is reached or an obstacle has been found.
*/
ICACHE_RAM_ATTR void limitSwitch() {
  Serial.println("SWITCH");

  if (rotation && ((millis() - lastDebounceTime) > debounceDelay)) {
    
    sendDoorMovement(DS_STOP);
    
    resetStepper();

    rotation = false;
  }
}