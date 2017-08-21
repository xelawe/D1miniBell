#ifndef MQTT_TOOL_H
#define MQTT_TOOL_H
 
#include <PubSubClient.h>
#include "cy_mqtt_cfg.h"


//const char* mqtt_subtopic_bell = "ATSH28/OG/G1/BELL/1/set";
const char* mqtt_pubtopic_bell = "ATSH28/OG/G1/BELL/1/state";
const char* mqtt_clientname;
const int* my_cnt_subtopics;
const char** my_mqtt_subtopics;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void reconnect_mqtt() {
  // Loop until we're reconnected
  //while (!client.connected()) {
  DebugPrint("Attempting MQTT connection...");
  // Attempt to connect
  if (client.connect("D1miniBell")) {
    DebugPrintln("connected");
    // Once connected, publish an announcement, retained
    //client.publish(mqtt_pubtopic, "hello world");

for (int i=0;i<*my_cnt_subtopics;i++){
  

    // ... and resubscribe
     client.subscribe(my_mqtt_subtopics[i]);
    }
  } else {
    DebugPrint("failed, rc=");
    DebugPrintln(client.state());
  }
}

void init_mqtt(const char* iv_clientname, const char** iv_subtopics, const int* iv_cnt_subtopics, MQTT_CALLBACK_SIGNATURE) {
  mqtt_clientname = iv_clientname;
  my_mqtt_subtopics = iv_subtopics;
  my_cnt_subtopics = iv_cnt_subtopics;
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void check_mqtt() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();
}

void pub_mqtt_toggle() {
  client.publish(mqtt_pubtopic_bell, "2");
}

#endif
