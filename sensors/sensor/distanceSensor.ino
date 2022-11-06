const int minDistanceObstacle = 3;
int countPossibleObstacle;

/**
* Calc of speed of sound
*/ 
float calcSpeedOfSound() {
  float newTemp = dht.readTemperature();
  if (!isnan(newTemp)) {
    temperature = newTemp;
  }

  return 331.3 + 0.606 * temperature;
}

/**
* Start distance sensor to check if exist an obstacle.
*/
void distanceSensor(int trigP, int echoP) {
  
  float speedOfSound = calcSpeedOfSound();
  countPossibleObstacle = 0;

  // send to other board the message i'm started
  Serial.print(ds_activated);
  bool exitLoop = false;

  while(!exitLoop) {

    if ( Serial.available() > 0 ) {
      char msg = Serial.read();
      // door is arrived at end point
      if (msg == ds_stop) {
        exitLoop = true;
      }
    }

    digitalWrite(trigP, LOW);
    delayMicroseconds(2);
    digitalWrite(trigP, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigP, LOW);

    duration = pulseIn(echoP, HIGH);
    float distance = (duration / 20000.0) * speedOfSound;

    if (distance > 1 && distance < minDistanceObstacle) {
      countPossibleObstacle += 1;
      exitLoop = isInterruptSended(countPossibleObstacle);
    }

    delay(100);
  }
}

bool isInterruptSended(int count) {

  if (count >= 3) {
    Serial.println("INVIO INTERRUPT");
    digitalWrite(obstaclePinSignal, LOW);
    delay(500);
    digitalWrite(obstaclePinSignal, HIGH); 
    return true;                 
  }

  return false;
}

