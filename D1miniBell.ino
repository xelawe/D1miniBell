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

const char* gv_hostname = "D1miniBell";

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Ticker.h>
#include "cy_wifi.h"
#include "cy_ota.h"
#include "cy_mqtt.h"

SoftwareSerial mySoftwareSerial(5, 4); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
Ticker TickerBell;


void printDetail(uint8_t type, int value);

void callback_mqtt1(char* topic, byte* payload, unsigned int length) {
  DebugPrintln("Callback 1 - Play tune");
  myDFPlayer.play((char)payload[0] - '0'); //Play # mp3
}

void callback_mqtt2(char* topic, byte* payload, unsigned int length) {
  DebugPrintln("Callback 2 - Mute");
  myDFPlayer.pause();
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
  //myDFPlayer.play(1);  //Play the first mp3

  wifi_init(gv_hostname);
  delay(500);

  init_ota(gv_hostname);

  init_mqtt(gv_hostname);
  add_subtopic("ATSH28/OG/G1/BELL/1/set", callback_mqtt1);
  add_subtopic("ATSH28/OG/G1/BELL/1/mute", callback_mqtt2);

  //pinMode(BUILTIN_LED, OUTPUT);  // initialize onboard LED as output
}

void loop() {

  DebugPrintln(F("loop"));

  check_ota();

  check_mqtt();

  delay(200);                      // wait one second

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
