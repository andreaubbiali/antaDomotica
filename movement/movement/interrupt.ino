/**
* Interrupt when a limit is reached or an obstacle has been found.
*/
ICACHE_RAM_ATTR void limitSwitch() {

  if (rotation && ((millis() - lastDebounceTime) > debounceDelay)) {
    
    Serial.print(ds_stop);
    
    resetStepper();

    rotation = false;
  }
}