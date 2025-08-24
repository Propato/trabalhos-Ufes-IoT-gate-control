#ifndef MQTTSERVICE_H
#define MQTTSERVICE_H

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

class MQTTService {
public:
    bool setup();
    void loop();

    void requestSlot();
    bool awaitResponse(unsigned long timeout = 10000);

    void sendExit(const String& data);

    String getSlot() const { return slot; }
    void setSlot(const String &newSlot) { slot = newSlot; }
    void resetSlot() { slot = "null"; }

    bool isConnected() { return mqtt_client.connected(); }

private:
    WiFiClient espClient;
    PubSubClient mqtt_client{espClient};

    String client_id;
    String slot = "null";

    const char *mqtt_broker = "broker.hivemq.com";
    const char *mqtt_base_topic = "propato/ufes/iot/projeto";
    const char *DEVICE_ID = "G1";
    const int mqtt_port = 1883;

    static MQTTService* instance;

    bool connect();
    bool publishMessage(const String& topic, const String& message);
    static void internalCallback(char *topic, byte *payload, unsigned int length);
};

#endif