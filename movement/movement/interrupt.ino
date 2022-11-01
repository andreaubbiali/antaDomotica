/**
* Interrupt when a limit is reached or an obstacle has been found.
*/
ICACHE_RAM_ATTR void limitSwitch() {

  if (rotation && ((millis() - lastDebounceTime) > debounceDelay)) {
    Serial.println("FINE CORSA CLICCATO");
    
    resetStepper();

    rotation = false;
  }
}