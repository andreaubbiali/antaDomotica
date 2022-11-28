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
      } else if (payload == ACTIVATE_DS_OPEN) {
        trigP = trigPinOpen;
        echoP = echoPinOpen;
      }

      checkSensor = true;

    }
    
  });

}

/**
* Publish
*/
void sendSensorStartMessage() {
  mqttClient.publish(applicationPrefix + "distanceSensor", DS_ACTIVATED);
}

void sendTemperature() {
  mqttClient.publish(applicationPrefix + "temperature", String(temperature), true);

  float humidity = dht.readHumidity();
  mqttClient.publish(applicationPrefix + "humidity", String(humidity), true);
}
