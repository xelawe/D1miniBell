#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySoftwareSerial(5, 4); // RX, TX
DFRobotDFPlayerMini myDFPlayer;


int tune_triggered = 999;


//void printDetail(uint8_t type, int value);

void init_dfplayer() {
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
  //myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  //myDFPlayer.play(0);  //Play the first mp3
  myDFPlayer.pause();
}


void play_DFplayer(int tunen_nbr) {
  // myDFPlayer.play((char)payload[0] - '0'); //Play # mp3
  myDFPlayer.play(tunen_nbr); //Play # mp3

  tune_triggered = tunen_nbr;
  delay(4000);
  myDFPlayer.pause();
}

void check_DFPlayer() {
  if (tune_triggered < 999) {
    pub_tune( tune_triggered );
    tune_triggered = 999;
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
