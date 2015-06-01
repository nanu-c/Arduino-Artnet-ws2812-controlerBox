//Nanu's ArtNet controler
//2015 MIT License
//nanu-c.org

//main loop at the end
//pins 22-28 for switchesanalog values
//pin A11-A15 for
bool debugg = false;
//For the display
#include <Wire.h>
#include <LCD.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR    0x27


//ethernet
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

//LEDS
#include "FastLED.h"
#define DATA_PIN 49

//Artnet
#define short_get_high_byte(x) ((HIGH_BYTE & x) >> 8)
#define short_get_low_byte(x)  (LOW_BYTE & x)
#define bytes_to_short(h,l) ( ((h << 8) & 0xff00) | (l & 0x00FF) );

byte mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0x4C, 0x8C} ; //the mac adress in HEX of ethernet shield or uno shield board
byte ip[] = {192, 168, 20, 100}; // the IP adress of your device, that should be in same universe of the network you are using

// the next two variables are set when a packet is received
byte remoteIp[4];        // holds received packet's originating IP
unsigned int remotePort; // holds received packet's originating port

//customisation: Artnet SubnetID + UniverseID
//edit this with SubnetID + UniverseID you want to receive
byte SubnetID = {1};
byte UniverseID = {1};
byte UniverseID2 = {2};
byte UniverseID3 = {3};

short select_universe = ((SubnetID * 16) + UniverseID);
short select_universe2 = ((SubnetID * 16) + UniverseID2);
short select_universe3 = ((SubnetID * 16) + UniverseID3);
//customisation: edit this if you want for example read and copy only 4 or 6 channels from channel 12 or 48 or whatever.
const int number_of_channels = 512; //512 for 512 channels
const int start_address = 0; // 0 if you want to read from channel 1

//buffers
const int MAX_BUFFER_UDP = 730;
char packetBuffer[MAX_BUFFER_UDP]; //buffer to store incoming data


// art net parameters
// artnet UDP port is by default 6454
const int art_net_header_size = 17;
const int max_packet_size = 576;
char ArtNetHead[8] = "Art-Net";
char OpHbyteReceive = 0;
char OpLbyteReceive = 0;
//short is_artnet_version_1=0;
//short is_artnet_version_2=0;
//short seq_artnet=0;
//short artnet_physical=0;
short incoming_universe = 0;
boolean is_opcode_is_dmx = 0;
boolean is_opcode_is_artpoll = 0;
boolean match_artnet = 1;
short Opcode = 0;
EthernetUDP Udp;
#define NUM_LEDS 500
CRGB leds[NUM_LEDS];
#define BACKLIGHT_PIN  3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

#define  LED_OFF  1
#define  LED_ON  0

/*-----( Declare objects )-----*/
//Mega2560	20 (SDA), 21 (SCL)
LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);
int inPin = 28; //pin for artnet/manual switch
boolean useArtNet = true;
unsigned long DELAY_TIME = 260;
String programNames[9] = {
  "BACKOUT    ",      //0
  "blink      ",
  "1:fade R       ",  //2
  "2:fade B       ",
  "3:fade G       ",  //4
  "4:fade Man.    ",
  "5:rainbow      ",
  "6:raupe        ",
  "7:slide        ",
};
byte tick[8] = {
  B00000,
  B00000,
  B00001,
  B00011,
  B10111,
  B01110,
  B00100,
};
byte progress1[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
};
byte progress2[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
};
byte progress3[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
};
byte progress4[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
};
byte progress5[8] = {
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};
byte progress6[8] = {
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};
byte progress7[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};
int resetCheck=0;
int resetPin=48;
//Announce classes
class Led_progs;
/* --- Main Setup */
void setup() {
  Serial.begin(115200);
  pinMode(inPin, INPUT);
  for (int i = 22; i <= 30; i++) {
    pinMode(i, OUTPUT);
  }
  //setup pins as PWM output
  lcd.begin (16, 2); // initialize the lcd
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, NEGATIVE);
  lcd.setBacklight(LED_ON);
  lcd.clear();

  delay(1000);
    FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS);
  lcd.createChar(0, tick);
  lcd.createChar(1, progress1);
  lcd.createChar(2, progress2);
  lcd.createChar(3, progress3);
  lcd.createChar(4, progress4);
  lcd.createChar(5, progress5);
  lcd.createChar(6, progress6);
  lcd.createChar(7, progress7);
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("Nanu-LedCont");
  //debug
  if ( digitalRead(25))debugg = true;
  //Artnet or Not
  if (digitalRead(inPin) == HIGH) {
    useArtNet = false;
    lcd.print("M");
      memset(leds, 0,  NUM_LEDS * sizeof(struct CRGB));
  }
  else {
    //setup ethernet and udp socket
    Ethernet.begin(mac, ip);
    Udp.begin(6454);
    Serial.println(Ethernet.localIP());
    lcd.setCursor(0, 1);
    lcd.println(Ethernet.localIP());
  }
  //the Leds


}
class Led_progs {
  public:
    Led_progs();
    //LED-progs
    void blink();
    void fadeColor(String); // r or g or b
    void fadeValues();
    void rainbow();
    void raupe();
    void slide();
    //special-progs
    void main();
    void blackOut();
    void readSensors();
    void setNameOnDisplay(String);
    void setCurrentProg(String);
    void menuSel();
    void menuPrint(int);
    void Wheel(int,byte);
    //values
    int sensorValuesSwitches[11];
    int sensorValuesAnalog[5];
    String currentProgram;
    int menuPosition;
    int nextProgram;
    int prgramNumbers;
    unsigned long upStop;
    unsigned long downStop;
    int speedPrint;
};

Led_progs::Led_progs() {
  menuPosition = 1;
  nextProgram = 0;
  prgramNumbers = 6;



}



/////////////////////////////////////////////////////////////////////////////

///////////////////////////////
void Led_progs::main() {
  readSensors();
  if (sensorValuesSwitches[0] == 1)fadeColor("r");
  else if (sensorValuesSwitches[1] == 1)fadeColor("g");
  else if (sensorValuesSwitches[2] == 1)fadeColor("b");
  else if (sensorValuesSwitches[3] == 1)blackOut();
  else if (sensorValuesSwitches[8] == 1){slide();sensorValuesSwitches[8]=0;}
  else if (sensorValuesSwitches[9] == 1){blink();sensorValuesSwitches[9]=0;}
  else if (nextProgram > 0) {
    switch (nextProgram) {
      case 1:   fadeColor("r"); break;
      case 2:   fadeColor("g"); break;
      case 3:   fadeColor("b"); break;
      case 4:   fadeValues(); break;
      case 5:   rainbow(); break;
      case 6:   raupe();break;
    }
   
  }
}

  //Main LOOOOOOOOOOOOOOOOOOOOOOOOOOP
  Led_progs led_prog;

  void loop() {
    reset();
    if (debugg) {
      lcd.setCursor(0, 0);
      lcd.print("debugg-m");

        test();
      
    }
    else {
      if (useArtNet) {

        useTheArtnet();
      }
      else {

        led_prog.main();
      }
    }
  }
  //////////////////////////////////////////////
  //////////////////////////////////////////////


