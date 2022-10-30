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
void distanceSensor() {
  
  float speedOfSound = calcSpeedOfSound();

  countPossibleObstacle = 0;

  while (checkDistance) {

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    float distance = (duration / 20000.0) * speedOfSound;
    if (distance > 1 && distance < minDistanceObstacle) {
        countPossibleObstacle += 1;
        if (countPossibleObstacle >= 3) {
          Serial.println("INVIO INTERRUPT");
          digitalWrite(obstaclePinSignal, LOW);
          delay(500);
          digitalWrite(obstaclePinSignal, HIGH); 
          countPossibleObstacle = 0;                   
        }
    }
    // if (distance <= minDistanceObstacle && distance >= 2) {
    // // if (distance <= minDistanceObstacle) {
    //   Serial.println("ERRORE FERMATI");
    // }
    delay(100);
  }
}

