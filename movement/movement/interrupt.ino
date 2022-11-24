/**
* Interrupt when a limit is reached or an obstacle has been found.
*/
ICACHE_RAM_ATTR void limitSwitch() {

  if (rotation && ((millis() - lastDebounceTimeSwitch) > debounceDelay)) {
    
    sendDoorMovement(DS_STOP);
    
    resetStepper();

    rotation = false;
  }
}

/**
* Interrupt when the man/aut button is pressed.
*/
uint lastDebounceTimeManAut = millis();
ICACHE_RAM_ATTR void manAutInterrupt() {

  if ((millis()-lastDebounceTimeManAut) >= debounceDelay){
    lastDebounceTimeManAut = millis();

    setIsManual(!isManual);
  }
}