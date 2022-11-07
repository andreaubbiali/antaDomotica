const String applicationPrefix = "antaDomotica/aubbiali/";

// /**
// * MQTT Subscriptions 
// */

/**
* Subscriptions to topics.
*/
void onConnectionEstablished() {

  mqttClient.subscribe(applicationPrefix + "moveDoor", [](const String& payload) {
    if (isManual) {
      if (payload == "close"){
        Serial.println("RECEIVED CLOSE FROM MQTT");
        // closeDoor();
      } else if (payload == "open"){
        // openDoor();
        Serial.println("RECEIVED OPEN FROM MQTT");
      } 
    }
  });

  mqttClient.subscribe(applicationPrefix + "setManual", [](const String& payload) {
    if (payload == "true") {
      isManual = true;
    } else if (payload == "false") {
      isManual = false;
    }
  });
}

/**
* Send the actual status of the door.
*/
void sendDoorMQTTDoorStatus(String msg) {
  // send message with retained true
  mqttClient.publish(applicationPrefix + "doorStatus", msg);
}