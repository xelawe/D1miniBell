#ifndef MQTT_TOOL_H
#define MQTT_TOOL_H

#include <PubSubClient.h>
#include "cy_mqtt_cfg.h"


const char* mqtt_subtopic_bell = "ATSH28/OG/G1/BELL/1/set";
const char* mqtt_pubtopic_bell = "ATSH28/OG/G1/BELL/1/state";
const char* mqtt_clientname;

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

    // ... and resubscribe
    //client.subscribe(mqtt_subtopic);
    client.subscribe(mqtt_subtopic_bell);
  } else {
    DebugPrint("failed, rc=");
    DebugPrintln(client.state());
    //DebugPrintln(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    //delay(5000);
    //}
  }
}

void init_mqtt(const char* iv_clientname, MQTT_CALLBACK_SIGNATURE) {
  mqtt_clientname = iv_clientname;
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
