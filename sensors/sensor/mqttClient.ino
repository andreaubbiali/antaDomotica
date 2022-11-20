const String applicationPrefix = "antaDomotica/aubbiali/";

/**
* Subscriptions
*/
void onConnectionEstablished() {

  mqttClient.subscribe(applicationPrefix + "doorMovement", [](const String& payload) {

    if (payload == DS_STOP) {
      checkSensor = false;
    } else {

      if (payload == ACTIVATE_DS_CLOSE) {
        trigP = trigPinClose;
        echoP = echoPinClose;
        Serial.println("CLOSE RECEIVED");
        // distanceSensor(trigPinClose, echoPinClose);
      } else if (payload == ACTIVATE_DS_OPEN) {
        trigP = trigPinClose;
        echoP = echoPinClose;
        Serial.println("OPEN RECEIVED");
        // distanceSensor(trigPinClose, echoPinClose);
      }

      checkSensor = true;

    }
    
  });

}

/**
* Publish
*/
void sendSensorStartMessage() {
  // send message with retained true
  mqttClient.publish(applicationPrefix + "distanceSensor", DS_ACTIVATED);
}