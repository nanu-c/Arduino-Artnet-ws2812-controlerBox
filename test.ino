void test() {
  String set = "";
  for (int i = 22; i <= 32; i++) {
    set += digitalRead(i);


  }
  Serial.println(set);
  lcd.setCursor(5, 1);
  lcd.print(set);
  testAnalogHelperMap(analogRead(A11),0);
  testAnalogHelperMap(analogRead(A12),1);
  testAnalogHelperMap(analogRead(A13),2);
  testAnalogHelperMap(analogRead(A14),3);
  testAnalogHelperMap(analogRead(A15),4);
  delay(100);
}
void testAnalogHelperMap(int getValue, int getPosition) {
  lcd.setCursor(getPosition, 1);
  switch (map( getValue , 0, 1024, 1, 7)) {
    case 1: lcd.write(byte(1)); break;
    case 2: lcd.write(byte(2)); break;
    case 3: lcd.write(byte(3)); break;
    case 4: lcd.write(byte(4)); break;
    case 5: lcd.write(byte(5)); break;
    case 6: lcd.write(byte(6)); break;
    case 7: lcd.write(byte(7)); break;
  }
 
}
