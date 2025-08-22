#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H
#include <PubSubClient.h>
class MQTTManager {
public:
    void setup();
    void loop();
    void requestSlot();
    bool hasSlot();
    String getSlot();
    void sendExit(const String& data);
private:
    WiFiClient espClient;
    PubSubClient mqtt_client;
    String slot;
    bool slotReceived = false;
};
#endif
