// If the value readed from photoresistence is under this constant, the door should be closed
const uint photoResistenceLimit = 250;
const uint readPhotoResistenceMinutes = 1;

/**
* Read photoresistence
*/
String readPhotoresistence() {
  uint startRead = millis();
  uint closeCount = 0;
  uint openCount = 0;
  String resp;

  // read until you reach n minutes of read or switch from automatic to manual (button pressed) 
  while(!isManual && (millis() < (startRead + (readPhotoResistenceMinutes*60000)))){
    uint val = analogRead(A0);
    if (val < photoResistenceLimit) {
      closeCount++;
    } else {
      openCount++;
    }
    delay(200);
    mqttClient.loop();
  }

  if (closeCount>openCount){
    resp = CLOSE;
  } else {
    resp = OPEN;
  }

  sendPhotoresistenceResp(resp);
  return resp;
}