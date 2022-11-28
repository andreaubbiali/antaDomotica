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
        closeDoor();
      } else if (payload == "open"){
        openDoor();
      } 
    }
  });

  mqttClient.subscribe(applicationPrefix + "setManual", [](const String& payload) {
    if (payload == "1") {
      setIsManual(true);
    } else if (payload == "0") {
      setIsManual(false);
    }   
  });

  mqttClient.subscribe(applicationPrefix + "distanceSensor", [](const String& payload) {
    if (payload == DS_ACTIVATED) {
      sensorResponse = true;
    }
  });

  /**
  * Set evening close time (the time must be of type HH)
  */
  mqttClient.subscribe(applicationPrefix + "setCloseTime", [](const String& payload) {
    if (isCorrectTime(payload)){
      setCloseTime(payload);
    }
  });

  /**
  * Set morning open time (the time must be of type HH)
  */
  mqttClient.subscribe(applicationPrefix + "setOpenTime", [](const String& payload) {
    if (isCorrectTime(payload)){
      setOpenTime(payload);
    }
  });
  
}

/**
* MQTT PUBLISH:
*/

/**
* Send the actual status of the door.
*/
void sendDoorMQTTDoorStatus(String msg) {
  mqttClient.publish(applicationPrefix + "doorStatus", msg, true);
}

void sendDoorMovement(String msg) {
  mqttClient.publish(applicationPrefix + "doorMovement", msg);
}

void sendUpdateCloseTime(int hours) {
  mqttClient.publish(applicationPrefix + "closeTime", String(hours) + ":00", true);
} 

void sendUpdateOpenTime(int hours) {
  mqttClient.publish(applicationPrefix + "openTime", String(hours) + ":00", true);
}

void sendUpdateIsManual(){
  mqttClient.publish(applicationPrefix + "isManual", String(isManual), true);
}

void sendPhotoresistenceResp(String resp){
  mqttClient.publish(applicationPrefix + "photoResistenceResp", resp);
}
