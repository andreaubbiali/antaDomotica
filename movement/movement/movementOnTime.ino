/**
* Movement based on the time specified
*/
int closeHour, openHour = 0;

void setupNTP(){
  WiFi.begin(WIFI_SSID, WIFI_PSW);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  timeClient.begin();
  timeClient.setTimeOffset(3600);

  // set default close time
  String time = CLOSE_TIME;
  if(!isCorrectTime(time)){
    time = "22";
  }
  setCloseTime(time);

  // set default open time
  time = OPEN_TIME;
  if(!isCorrectTime(time)){
    time = "06";
  }
  setOpenTime(time);
}

/**
* Return the status the door should be based on the time.
* If is in the interval from open to close -> open
*/
String movementOnTime() {
  timeClient.update();
  int currentHour = timeClient.getHours();

  if (currentHour >= openHour && currentHour < closeHour){
    return OPEN;     
  }

  return CLOSE;
}

/**
* Check if a time is in the correct format (is an integer number between 0 and 24)
*/
bool isCorrectTime(String time){

  uint hours = time.toInt();
  if ((hours == 0 && time != "0") || hours > 24){
    return false;    
  }
    
  return true;
}

// SETTER
/**
* Set close time checking its correctness with the open time
* !ATTENTION! call this method only after checking isCorrectTime
*/
void setCloseTime(String time){
  uint tmpHour = time.toInt();

  if (tmpHour < openHour) {
    return;
  }

  closeHour = tmpHour;
  sendUpdateCloseTime(closeHour);
}

/**
* Set open time checking its correctness with the close time
* !ATTENTION! call this method only after checking isCorrectTime
*/
void setOpenTime(String time){
  int tmpHour = time.toInt();

  if (tmpHour > closeHour){
    return;
  }
  
  openHour = tmpHour;
  sendUpdateOpenTime(openHour);
}

