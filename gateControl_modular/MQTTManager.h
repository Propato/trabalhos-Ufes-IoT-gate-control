#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

class MQTTManager {
public:
    void setup();
    void loop();

    void requestSlot();
    void awaitResponse();


    void sendExit(const String& data);

    String getSlot() const { return slot; }
    void resetSlot() const { slot = "null"; }

    bool isConnected() const { return connected; }

private:
    WiFiClient espClient;
    PubSubClient mqtt_client;

    String client_id;
    String slot = "null";

    bool connected = false;

    bool connect();
    void publishMessage(const String& topic, const String& message);
    void internalCallback(char *topic, byte *payload, unsigned int length);
};

#endif
