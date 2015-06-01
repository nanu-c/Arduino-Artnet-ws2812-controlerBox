void Led_progs::blackOut() {
  if (digitalRead(25) == HIGH) {
    setCurrentProg(programNames[0]);

    LEDS.showColor(CRGB(0, 0, 0));
    while (digitalRead(25) == HIGH) {
      delay(120);
    }
  }
}
void Led_progs::blink() {
  setCurrentProg(programNames[0]);
  LEDS.showColor(CRGB(sensorValuesAnalog[0] / 4, sensorValuesAnalog[1] / 4, sensorValuesAnalog[2] / 4));
  delay(20);
  LEDS.showColor(CRGB(0, 0, 0));
  delay(200);
  readSensors();
}
void Led_progs::fadeColor(String color) {

  // fade up
  if (color == "r") {
    setCurrentProg(programNames[2]);
    for (int x = 0; x < 128; x++) {
      // The showColor method sets all the leds in the strip to the same color
      LEDS.showColor(CRGB(x, 0, 0));
      delay(DELAY_TIME / 10); readSensors();
    }
    readSensors();
    // fade down
    for (int x = 128; x >= 0; x--) {
      LEDS.showColor(CRGB(x, 0, 0));
      delay(DELAY_TIME / 10); readSensors();
    }
  }
  else if (color == "g") {
    setCurrentProg(programNames[3]);
    for (int x = 0; x < 128; x++) {
      // The showColor method sets all the leds in the strip to the same color
      LEDS.showColor(CRGB(0, x, 0));
      delay(DELAY_TIME / 10); readSensors();
    }
    readSensors();
    // fade down
    for (int x = 128; x >= 0; x--) {
      LEDS.showColor(CRGB(0, x, 0));
      delay(DELAY_TIME / 10);readSensors();
    }
  }
  else if (color == "b") {
    setCurrentProg( (String) programNames[4][2 - 11]);
    for (int x = 0; x < 128; x++) {
      // The showColor method sets all the leds in the strip to the same color
      LEDS.showColor(CRGB(0, 0, x));
      delay(DELAY_TIME / 10);readSensors();
    }
    readSensors();
    // fade down
    for (int x = 128; x >= 0; x--) {
      LEDS.showColor(CRGB(0, 0, x));
      delay(DELAY_TIME / 10); readSensors();
    }
  }
}
void Led_progs::fadeValues() {
  setCurrentProg(programNames[5]);
  int red = 0;
  int green = 0;
  int blue = 0;
  // fade up
  int change = 0;

  for (int x = 0; x < 255; x++) {
    change = 0;
    delay(DELAY_TIME / 10);
    if (x <= sensorValuesAnalog[0] / 4) {
      red = x;
      change = 1;
    }
    if (x <= sensorValuesAnalog[1] / 4) {
      green = x;
      change = 1;
    }
    if (x <= sensorValuesAnalog[2] / 4) {
      blue = x;
      change = 1;
    }
    if (change == 0) {
      break;
    }
    readSensors();
    LEDS.showColor(CRGB(red, green, blue));

  }

  int maxColor = sensorValuesAnalog[0] / 4;
  if (sensorValuesAnalog[1]  / 4 > maxColor) {
    maxColor = sensorValuesAnalog[1]  / 4;
  }
  if (sensorValuesAnalog[2] / 4 > maxColor) {
    maxColor = sensorValuesAnalog[2] / 4;
  }
  // fade down
  for (int x = maxColor; x >= 0; x--) {
    delay(DELAY_TIME / 10); readSensors();
    if (x <= sensorValuesAnalog[0] / 4) {
      red = x;
    }
    if (x <= sensorValuesAnalog[1] / 4) {
      green = x;
    }
    if (x <= sensorValuesAnalog[2] / 4) {
      blue = x;
    }
    LEDS.showColor(CRGB(red, green, blue));
    //delay(analogRead(sensorPin3)/2);
  }
}
void Led_progs::Wheel(int k, byte WheelPos) {
  if (WheelPos < 85) {

    leds[k].r = WheelPos * 3;
    leds[k].g = 225 - WheelPos * 3;
    leds[k].b = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;

    leds[k].r = 255 - WheelPos * 3;
    leds[k].g = 0;
    leds[k].b = WheelPos * 3;
  } else {
    WheelPos -= 170;
    leds[k].r = 0;
    leds[k].g = WheelPos * 3;
    leds[k].b = 255 - WheelPos * 3;
  }
}
void Led_progs::rainbow() {
  setCurrentProg(programNames[6]);
  uint16_t i, j;

  for (j = 0; j < 256 * 3; j++) {
    //for(i=0; i<NUM_LEDS; i++) {
    // Wheel(i,(i+j) & 255);
    //}
    for (i = 0; i < NUM_LEDS; i++) {
      Wheel(i, ((i * 256 / NUM_LEDS) + j) & 255);
    }
    LEDS.show();
    delay(DELAY_TIME); readSensors();
  }
}


void Led_progs::raupe() {
  setCurrentProg(programNames[7]);
  for (int j = 0; j <= NUM_LEDS - 20; j++) {

    leds[j + 1].r = 254;
    leds[j].r = 0;
    leds[j].g = 254;
    if (j > 20) {
      leds[j - 20].g = 0;
    }


    LEDS.show();
    delay(DELAY_TIME); readSensors();
  }
}
void Led_progs::slide() {
  setCurrentProg(programNames[8]);
  LEDS.showColor(CRGB(sensorValuesAnalog[0] / 4, sensorValuesAnalog[1] / 4, sensorValuesAnalog[2] / 4));
  delay(20);
  readSensors();
}
