//all the active artnet stuff
void useTheArtnet() {
  Serial.println(Ethernet.localIP());
  int packetSize = Udp.parsePacket();

  //lcd.print("                 ");

  //FIXME: test/debug check
  if (packetSize > art_net_header_size && packetSize <= max_packet_size) { //check size to avoid unneeded checks


    IPAddress remote = Udp.remoteIP();
    remotePort = Udp.remotePort();
    Udp.read(packetBuffer, MAX_BUFFER_UDP);

    //read header
    match_artnet = 1;
    for (int i = 0; i < 7; i++) {
      //if not corresponding, this is not an artnet packet, so we stop reading
      if (char(packetBuffer[i]) != ArtNetHead[i]) {
        match_artnet = 0; break;
      }
    }

    //if its an artnet header
    if (match_artnet == 1) {
      //artnet protocole revision, not really needed
      //is_artnet_version_1=packetBuffer[10];
      //is_artnet_version_2=packetBuffer[11];*/

      //sequence of data, to avoid lost packets on routeurs
      //seq_artnet=packetBuffer[12];*/

      //physical port of  dmx N°
      //artnet_physical=packetBuffer[13];*/

      //operator code enables to know wich type of message Art-Net it is
      Opcode = bytes_to_short(packetBuffer[9], packetBuffer[8]);

      //if opcode is DMX type
      if (Opcode == 0x5000) {
        is_opcode_is_dmx = 1; is_opcode_is_artpoll = 0;
      }

      //if opcode is artpoll
      else if (Opcode == 0x2000) {
        is_opcode_is_artpoll = 1; is_opcode_is_dmx = 0;
        //( we should normally reply to it, giving ip adress of the device)
      }

      //if its DMX data we will read it now
      if (is_opcode_is_dmx = 1) {
        //gotdata();
        //read incoming universe
        incoming_universe = bytes_to_short(packetBuffer[15], packetBuffer[14])
                            //lcd.print("         ");
                            lcd.setCursor(0, 0);
        String joinS = "Universe: ";
        joinS += incoming_universe;
        joinS += "    ";
        lcd.print(joinS);
  
        //if it is selected universe DMX will be read
        if (incoming_universe == UniverseID) setLEDS(0);
        if (incoming_universe == UniverseID2) setLEDS(172);
        if (incoming_universe == UniverseID3) setLEDS(341);

      }
    }//end of sniffing

  }
  FastLED.show();
}
void setLEDS(int startled) {

  int j = startled;
  lcd.setCursor(13, 1);
  //lcd.print(packetBuffer[art_net_header_size + 2]);
  lcd.print(startled);
  for (int i = start_address; i < number_of_channels; i = i + 3) {
    if (j <= NUM_LEDS) {
      leds[j].r = byte(packetBuffer[i + art_net_header_size + 1]);
      leds[j].g = byte(packetBuffer[i + art_net_header_size + 2]);
      leds[j].b = byte(packetBuffer[i + art_net_header_size + 3]);
    }
    j++;
    // buffer_channel_arduino[i-start_address]= byte(packetBuffer[i+art_net_header_size+1]);
  }
  FastLED.show();
}
boolean gotdataC = false;
void gotdata() {
  if (!gotdata) {
    gotdataC = true;
    lcd.setCursor(0, 0);
    lcd.print("Universe: ");
  }

}
