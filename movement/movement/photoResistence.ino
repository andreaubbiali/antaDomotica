const uint photoResistenceUpperLimit = 100;
const uint photoResistenceLowerLimit = 50;

/**
* Read photoresistence
*/
void readPhotoresistence() {

  if (countPhotoResistenceUp == 4) {
    // closeDoor();
    countPhotoResistenceUp = 0;
  } else if (countPhotoResistenceLow == 4) {
    // openDoor();
    countPhotoResistenceLow = 0;
  }
  
  uint val = analogRead(A0);
  if (val > photoResistenceUpperLimit) {
    countPhotoResistenceUp += 1;
  }

  if (val < photoResistenceLowerLimit) {
    countPhotoResistenceLow += 1;
  }
}