const int minDistanceObstacle = 3;
int countPossibleObstacle;
const int maximumPossibleObstacle = 3;

/**
* Populate (and send to mqtt) the temperature field with the new temperature if is present otherwise mantain the old temperature.
* 
*/
void readTemperature(){
  float temp = dht.readTemperature();
  if (!isnan(temp)) {
    temperature = temp;
    sendTemperature();
  }

}

/**
* Calc of speed of sound
*/ 
float calcSpeedOfSound() {
  readTemperature();

  return 331.3 + 0.606 * temperature;
}

/**
* Start distance sensor to check if exist an obstacle.
*/
void distanceSensor(){

  float speedOfSound = calcSpeedOfSound();
  countPossibleObstacle = 0;
  sendSensorStartMessage();

  while(checkSensor){
    digitalWrite(trigP, LOW);
    delayMicroseconds(2);
    digitalWrite(trigP, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigP, LOW);

    duration = pulseIn(echoP, HIGH);
    float distance = (duration / 20000.0) * speedOfSound;

    if (distance > 1 && distance < minDistanceObstacle) {
      countPossibleObstacle += 1;
      checkSensor = !isInterruptSended(countPossibleObstacle);
    }

    mqttClient.loop();
    delay(100);
  }
}

/**
* Send interrupt (to stop the movement) if count is bigger than the maximum possible obstacle counter
*/
bool isInterruptSended(int count) {

  if (count >= maximumPossibleObstacle) {
    digitalWrite(obstaclePinSignal, LOW);
    delay(500);
    digitalWrite(obstaclePinSignal, HIGH); 
    return true;                 
  }

  return false;
}

