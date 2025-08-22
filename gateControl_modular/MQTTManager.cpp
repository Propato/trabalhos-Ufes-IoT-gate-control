#include "MQTTManager.h"
const char *mqtt_broker = "broker.hivemq.com";
const char *mqtt_base_topic = "propato/ufes/iot/projeto";
const char *DEVICE_ID = "G1";
const int mqtt_port = 1883;

void MQTTManager::setup() {
    mqtt_client = PubSubClient(espClient);
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    // mqtt_client.setCallback(...); // Implementar callback
    mqtt_client.connect("esp8266-client");
}

void MQTTManager::loop() {
    mqtt_client.loop();
}

void MQTTManager::requestSlot() {
    char request_topic[100];
    snprintf(request_topic, sizeof(request_topic), "%s/parking/entry", mqtt_base_topic);
    mqtt_client.publish(request_topic, DEVICE_ID);
    // slotReceived = false; // Resetar flag
}

bool MQTTManager::hasSlot() {
    return slotReceived;
}

String MQTTManager::getSlot() {
    return slot;
}

void MQTTManager::sendExit(const String& data) {
    char exit_topic[100];
    snprintf(exit_topic, sizeof(exit_topic), "%s/parking/exit", mqtt_base_topic);
    mqtt_client.publish(exit_topic, data.c_str());
}
