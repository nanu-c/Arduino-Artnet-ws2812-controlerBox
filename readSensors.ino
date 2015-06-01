//Sensors
void Led_progs::readSensors() {
  sensorValuesSwitches[0] = digitalRead(22);
  sensorValuesSwitches[1] = digitalRead(23);
  sensorValuesSwitches[2] = digitalRead(24);
  sensorValuesSwitches[3] = digitalRead(25);
  unsigned long milSec=millis();
  if (digitalRead(26)==1&&(milSec-upStop)>300){
    sensorValuesSwitches[4] =1;
    upStop=millis();
  }else sensorValuesSwitches[4] = 0;
  
  if (digitalRead(27)==1&&milSec-downStop>300){
    downStop=millis();
    sensorValuesSwitches[5] = 1;
    
  }else sensorValuesSwitches[5] = 0;
  if (digitalRead(26)==1&&digitalRead(27)==1){
    sensorValuesSwitches[4] = 1;
    sensorValuesSwitches[5] = 1;
  }
  sensorValuesSwitches[6] = digitalRead(28);
  sensorValuesSwitches[7] = digitalRead(29);
  sensorValuesSwitches[8] = digitalRead(30);
  sensorValuesSwitches[9] = digitalRead(31);
  sensorValuesSwitches[10] = digitalRead(32);
  sensorValuesAnalog[0] = analogRead(A11);
  sensorValuesAnalog[1] = analogRead(A12);
  sensorValuesAnalog[2] = analogRead(A13);
  sensorValuesAnalog[3] = analogRead(A14);
testAnalogHelperMap(analogRead(A15),15);
  DELAY_TIME = analogRead(A15) / 10;
  blackOut();
  if (sensorValuesSwitches[4] == 1||sensorValuesSwitches[5] == 1)menuSel();

}
