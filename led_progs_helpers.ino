void Led_progs::setCurrentProg(String getName) {
  //blackOut();
  if (currentProgram != getName) {
    currentProgram = getName;
    setNameOnDisplay(getName);
  }
}
void Led_progs::setNameOnDisplay(String getName) {
  lcd.setCursor(0, 1);
  lcd.print(getName);
}
