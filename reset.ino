void reset(){
 if (digitalRead(26)==1){

  if(resetCheck!=0&&millis()-resetCheck>7000){
      lcd.setCursor(0, 0);
  lcd.print("reset");
  lcd.write(byte(7));
  lcd.write(byte(7));
  lcd.write(byte(7));
  lcd.write(byte(7));
  lcd.write(byte(7));
  lcd.write(byte(7));
  lcd.write(byte(7));
  lcd.write(byte(7));
  lcd.write(byte(7));
  lcd.write(byte(7));
     pinMode(resetPin, OUTPUT); 
    digitalWrite(resetPin, HIGH); 
  while(1){
  }  
  }
  else if(resetCheck==0)resetCheck=millis();
 } 
 else resetCheck=0;
}
