/**
* Movement based on the time specified
*/
int closeHour, closeMinutes, openHour = 0, openMinutes = 0;

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
    time = "22:00";
  }
  setCloseTime(time);

  // set default open time
  time = OPEN_TIME;
  if(!isCorrectTime(time)){
    time = "06:00";
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
  int currentMinutes = timeClient.getMinutes();

  if (currentHour >= openHour && currentMinutes >= openMinutes &&
      currentHour < closeHour && currentMinutes < closeMinutes){
    return OPEN;
  }

  return CLOSE;
}

/**
* Check if a time is in the correct format (hh:ss)
*/
bool isCorrectTime(String time){

  // check the separator
  int index = time.indexOf(":");
  if (index == -1 || index == (time.length()-1)){
    return false;
  }

  // substring not a number
  if (time.substring(0, index).toInt() == 0 && time.substring(0, index) != "0"){
    return false;
  }

  // substring not a number
  if (time.substring(index+1).toInt() == 0 && time.substring(index+1) != "0"){
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
  int tmpHour = getHourFromTime(time);
  int tmpMinutes = getMinutesFromTime(time);

  if (tmpHour < openHour) {
    return;
  }

  if (tmpHour == openHour && tmpMinutes <= openMinutes) {
    return;
  }

  closeHour = tmpHour;
  closeMinutes = tmpMinutes;

  sendUpdateCloseTime(closeHour, closeMinutes);
}

/**
* Set open time checking its correctness with the close time
* !ATTENTION! call this method only after checking isCorrectTime
*/
void setOpenTime(String time){
  int tmpHour = getHourFromTime(time);
  int tmpMinutes = getMinutesFromTime(time);

  if (tmpHour > closeHour){
    return;
  }

  if (tmpHour == closeHour && tmpMinutes >= closeMinutes) {
    return;
  }
  
  openHour = tmpHour;
  openMinutes = tmpMinutes;
  sendUpdateOpenTime(openHour, openMinutes);
}

// GETTER
/**
* Get hour from time string.
* !ATTENTION! call this method only after checking isCorrectTime
*/
int getHourFromTime(String time){
  return time.substring(0, 2).toInt();
}

/**
* Get minutes from time string.
* !ATTENTION! call this method only after checking isCorrectTime
*/
int getMinutesFromTime(String time){
  return time.substring(3, 5).toInt();
}

