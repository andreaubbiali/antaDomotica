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
        closeDoor();
      } else if (payload == "open"){
        openDoor();
        Serial.println("RECEIVED OPEN FROM MQTT");
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
    Serial.println("RISPOSTA RICEVUTA");
    Serial.println(DS_ACTIVATED);
    Serial.println(payload);
    
    if (payload == DS_ACTIVATED) {
      sensorResponse = true;
    }
  });

  /**
  * Set evening close time (the time must be of type HH:MM)
  */
  mqttClient.subscribe(applicationPrefix + "setCloseTime", [](const String& payload) {

    Serial.println("SET CLOSE TIME ARRIVED " + payload);
    if (isCorrectTime(payload)){
      Serial.println("ENTRA in set close time");
      Serial.println("SET CLOSE TIME");
      setCloseTime(payload);
    }

  });

  /**
  * Set morning open time (the time must be of type HH:MM)
  */
  mqttClient.subscribe(applicationPrefix + "setOpenTime", [](const String& payload) {

    if (isCorrectTime(payload)){
      Serial.println("SET OPEN TIME");
      setOpenTime(payload);
    }

  });
  
}

/**
* Send the actual status of the door.
*/
void sendDoorMQTTDoorStatus(String msg) {
  mqttClient.publish(applicationPrefix + "doorStatus", msg, true);
}

void sendDoorMovement(String msg) {
  mqttClient.publish(applicationPrefix + "doorMovement", msg);
}

void sendUpdateCloseTime(int hours, int minutes) {
  Serial.println("SEND UPDATE CLOSE TIME: " + String(hours) + ":" + String(minutes));
  mqttClient.publish(applicationPrefix + "closeTime", String(hours) + ":" + String(minutes), true);
} 

void sendUpdateOpenTime(int hours, int minutes) {
  mqttClient.publish(applicationPrefix + "openTime", String(hours) + ":" + String(minutes), true);
}

void sendUpdateIsManual(){
  mqttClient.publish(applicationPrefix + "isManual", String(isManual), true);
}

