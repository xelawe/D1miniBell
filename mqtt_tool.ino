#include "cy_mqtt_v1.h"

char gv_stopic_bell[MQTT_TOPSZ];
const char gc_stopic_bell[] PROGMEM = "bell";
const char* const gt_stopic[] PROGMEM = { gc_stopic_bell };
char gv_sbuffer[7];// buffer for reading the string to (needs to be large enough to take the longest string

char gv_ptopic[MQTT_TOPSZ];
char gv_pbuffer[5];// buffer for reading the string to (needs to be large enough to take the longest string

const char gc_cmd_off[] PROGMEM = "off";
const char gc_cmd_on[] PROGMEM = "on";
const char gc_cmd_mute[] PROGMEM = "mute";
const char* const gt_cmd[] PROGMEM = { gc_cmd_off, gc_cmd_on, gc_cmd_mute, "3", "4", "5", "6" };

char *get_stopic_ix( int ix ) {
  strcpy_P(gv_sbuffer, (char*)pgm_read_dword(&(gt_stopic[ix])));
  //strcpy_P(gv_buffer, );
  return gv_sbuffer;
}

char *get_cmd_ix( int ix ) {
  strcpy_P(gv_pbuffer, (char*)pgm_read_dword(&(gt_cmd[ix])));
  //strcpy_P(gv_buffer, );
  return gv_pbuffer;
}


void callback_mqtt1(char* topic, byte* payload, unsigned int length) {

  DebugPrintln("Callback 1 - Play tune");
  play_DFplayer((char)payload[0] - '0'); //Play # mp3

}

//void callback_mqtt2(char* topic, byte* payload, unsigned int length) {
//  DebugPrintln("Callback 2 - Mute");
//
//}

void init_mqtt_local() {
  init_mqtt(gv_clientname);
  //add_subtopic("ATSH28/OG/G1/BELL/1/set", callback_mqtt1);
  //add_subtopic("ATSH28/OG/G1/BELL/1/mute", callback_mqtt2);
  add_subtopic(mqtt_GetTopic_P(gv_stopic_bell, 0, gv_clientname, get_stopic_ix(0)), callback_mqtt1);
}

void pub_tune(int tune) {
  if (!client.publish(mqtt_GetTopic_P(gv_ptopic, 1, gv_clientname, get_stopic_ix(0)), get_cmd_ix(tune), false)) {
    DebugPrintln(F("pub failed!"));
  } else {
    DebugPrintln(F("pub ok!"));
  }
}

