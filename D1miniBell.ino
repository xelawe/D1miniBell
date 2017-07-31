/*
   MQTT Bell plays MP3 files
   Turns on the onboard LED on for one second, then off for one second, repeatedly.
   This uses delay() to pause between LED toggles.
*/
#define serdebug
#ifdef serdebug
#define DebugPrint(...) {  Serial.print(__VA_ARGS__); }
#define DebugPrintln(...) {  Serial.println(__VA_ARGS__); }
#else
#define DebugPrint(...) { }
#define DebugPrintln(...) { }
#endif

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Ticker.h>
#include "cy_wifi.h"
#include "cy_ota.h"
#include "mqtt_tool.h"

SoftwareSerial mySoftwareSerial(5, 4); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
Ticker TickerBell;


void printDetail(uint8_t type, int value);



void callback_mqtt(char* topic, byte* payload, unsigned int length) {
  DebugPrint("Message arrived [");
  DebugPrint(topic);
  DebugPrint("] ");
  for (int i = 0; i < length; i++) {
    DebugPrint((char)payload[i]);
  }
  DebugPrintln();

  // Switch on the LED if an 1 was received as first character
  switch ((char)payload[0]) {
    case '0':
      //turnOff();
      break;
    case '1':
      //turnOn();
      break;
    case '2':
      //toggle();
      break;
  }
}


void setup() {
#ifdef serdebug
  Serial.begin(115200);
#endif

  mySoftwareSerial.begin(9600);
  DebugPrintln();
  DebugPrintln(F("DFRobot DFPlayer Mini Demo"));
  DebugPrintln(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    DebugPrintln(F("Unable to begin:"));
    DebugPrintln(F("1.Please recheck the connection!"));
    DebugPrintln(F("2.Please insert the SD card!"));
    while (true);
  }
  DebugPrintln(F("DFPlayer Mini online."));


  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  myDFPlayer.play(1);  //Play the first mp3

  wifi_init("D1miniBell");
  delay(500);

  init_ota("D1miniBell");

  init_mqtt(callback_mqtt);


  //pinMode(BUILTIN_LED, OUTPUT);  // initialize onboard LED as output
}

void loop() {

  check_ota();

  //digitalWrite(BUILTIN_LED, HIGH);  // turn on LED with voltage HIGH
  //delay(1000);                      // wait one second
  //digitalWrite(BUILTIN_LED, LOW);   // turn off LED with voltage LOW
  delay(1000);                      // wait one second


  //  static unsigned long timer = millis();
  //
  //  if (millis() - timer > 3000) {
  //    timer = millis();
  //    myDFPlayer.next();  //Play next mp3 every 3 second.
  //  }
  //
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
