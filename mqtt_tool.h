

#ifndef MQTT_TOOL_H
#define MQTT_TOOL_H

#include <PubSubClient.h>
#include <LinkedList.h>
#include "cy_mqtt_cfg.h"



//const char* mqtt_subtopic_bell = "ATSH28/OG/G1/BELL/1/set";
//const char* mqtt_pubtopic_bell = "ATSH28/OG/G1/BELL/1/state";
const char* mqtt_clientname;

WiFiClient espClient;
PubSubClient client(espClient);

// Class for SubTopics
class MQTTSubTopic {
  public:
    const char *topic;
    MQTT_CALLBACK_SIGNATURE;
};

// List for SubTopics
LinkedList<MQTTSubTopic*> gv_SubTopicList = LinkedList<MQTTSubTopic*>();


// add Subtopic to internal linked list
void add_subtopic(const char* iv_subtopic, MQTT_CALLBACK_SIGNATURE) {
  // Create a SubTopic
  MQTTSubTopic *SubTopic = new MQTTSubTopic();
  // set Values
  SubTopic->topic = iv_subtopic;
  SubTopic->callback = callback;
  // add to list
  gv_SubTopicList.add(SubTopic);

}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  DebugPrint("Message arrived [");
  DebugPrint(topic);
  DebugPrint("] ");
  for (int i = 0; i < length; i++) {
    DebugPrint((char)payload[i]);
  }
  DebugPrintln();

    MQTTSubTopic *lv_SubTopic;
    for (int i = 0; i < gv_SubTopicList.size(); i++) {

      // is topic called?
      lv_SubTopic = gv_SubTopicList.get(i);
      if (lv_SubTopic->topic == topic){
        lv_SubTopic->callback(topic,payload,length);
      }
    }

//  for (int j = 0; j < cnt_subtopics; j++) {
//    if ( topic == mqtt_subtopics[j]) {
//      mqtt_flg_subtopics[j] = true;
//    }
//  }
}

void reconnect_mqtt() {
  // Loop until we're reconnected
  //while (!client.connected()) {
  DebugPrint("Attempting MQTT connection...");
  // Attempt to connect
  if (client.connect("D1miniBell")) {
    DebugPrintln("connected");
    // Once connected, publish an announcement, retained
    //client.publish(mqtt_pubtopic, "hello world");

    MQTTSubTopic *lv_SubTopic;
    for (int i = 0; i < gv_SubTopicList.size(); i++) {

      // ... and resubscribe
      lv_SubTopic = gv_SubTopicList.get(i);
      client.subscribe(lv_SubTopic->topic);
    }
  } else {
    DebugPrint("failed, rc=");
    DebugPrintln(client.state());
  }
}

void init_mqtt(const char* iv_clientname) {
  mqtt_clientname = iv_clientname;

  client.setServer(mqtt_server, 1883);
  client.setCallback(mqtt_callback);
}

void check_mqtt() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();
}

void pub_mqtt_toggle() {
  //  client.publish(mqtt_pubtopic_bell, "2");
}

#endif
