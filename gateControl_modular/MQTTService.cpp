// MQTTService.cpp
#include "MQTTService.h"

const char *mqtt_broker = "broker.hivemq.com";
const char *mqtt_base_topic = "propato/ufes/iot/projeto";
const char *DEVICE_ID = "G1";
const int mqtt_port = 1883;

void MQTTService::internalCallback(char *topic, byte *payload, unsigned int length) {
    char message[length + 1];
    for (unsigned int i = 0; i < length; i++) {
       message[i] = (char)payload[i];
    }
    message[length] = '\0';  // Null-terminate the string

    // Parse do JSON
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, message);

    if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return;
    }

    if (doc.containsKey("slot")) {
        slot = doc["slot"].as<String>();
    } else {
        Serial.println("JSON doesn't contain 'slot' key");
    }
}

void MQTTService::setup() {
    mqtt_client = PubSubClient(espClient);
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(internalCallback);

    connected = connect();
}

bool MQTTService::connect() {
    const int maxAttempts = 3;
    int attempt = 1;

    while (attempt <= maxAttempts) {
        client_id = "esp8266-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT Broker as %s (Attempt %d)...\n", client_id.c_str(), attempt);

        if (mqtt_client.connect(client_id.c_str())) {
            Serial.println("✅ Connected to MQTT broker");

            char response_topic[100];
            snprintf(response_topic, sizeof(response_topic),
                     "%s/parking/entry/%s/response", mqtt_base_topic, DEVICE_ID);
            mqtt_client.subscribe(response_topic);

            return true;
        } else {
            Serial.printf("❌ Failed (rc=%d). Retry in 5s...\n", mqtt_client.state());
            attempt++;
            delay(5000);
        }
    }

    Serial.println("❌ Could not connect to MQTT broker after 3 attempts.");
    return false;
}

void MQTTService::loop() {
    mqtt_client.loop();
}

void MQTTService::publishMessage(const String& topic, const String& message) {
    if (!connected) connected = connect();

    if (connected) {
        mqtt_client.publish(topic.c_str(), message.c_str());
    }
}

void MQTTService::requestSlot() {
    String topic = String(mqtt_base_topic) + "/parking/entry";
    publishMessage(topic, String(DEVICE_ID));
}

void MQTTService::sendExit(const String& data) {
    String topic = String(mqtt_base_topic) + "/parking/exit";
    publishMessage(topic, data);
}

void MQTTService::awaitResponse() {
    while (slot == "null") {
        mqtt_client.loop();
    }
}