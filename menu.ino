

void Led_progs::menuSel() {

  if (sensorValuesSwitches[5] == 1 && sensorValuesSwitches[4] == 1) {
    nextProgram = menuPosition;
    lcd.setCursor(14, 0);
    lcd.print((String) menuPosition);
    lcd.write(byte(0));
    sensorValuesSwitches[4] = 0;
    sensorValuesSwitches[5] = 0;
  }
  //up
  else if (sensorValuesSwitches[4] == 1) {
    --menuPosition;
    if (menuPosition < 1)menuPosition = prgramNumbers;
    menuPrint(menuPosition);
    sensorValuesSwitches[4] = 0;

  }
  else if (sensorValuesSwitches[5] == 1) {
    ++menuPosition;
    if (menuPosition > prgramNumbers)menuPosition = 1;
    menuPrint(menuPosition);
    sensorValuesSwitches[5] = 0;

  }

}
void Led_progs::menuPrint(int number) {
  lcd.setCursor(0, 0);
  lcd.print("S:" + programNames[number + 1]);
}
