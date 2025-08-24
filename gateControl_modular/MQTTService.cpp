#include "MQTTService.h"
#include <ArduinoJson.h>

MQTTService* MQTTService::instance = nullptr;

void MQTTService::internalCallback(char *topic, byte *payload, unsigned int length) {
    if (!instance) return;

    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    String topicStr(topic);
    String expectedTopic = String(instance->mqtt_base_topic) +
                          "/parking/entry/" +
                          String(instance->DEVICE_ID) +
                          "/response";

    if (topicStr == expectedTopic) {
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, message);

        if (error) {
            Serial.print("JSON parsing failed: ");
            Serial.println(error.c_str());
            return;
        }

        if (doc.containsKey("slot")) {
            instance->setSlot(doc["slot"].as<String>());
            Serial.printf("Occupied Slot: %s\n", instance->getSlot().c_str());
        } else {
            Serial.println("JSON doesn't contain 'slot' key");
        }
    }
}

bool MQTTService::setup() {
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setBufferSize(512); // Aumentar buffer size

    instance = this;
    mqtt_client.setCallback(MQTTService::internalCallback);

    return connect();
}

bool MQTTService::connect() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected. Cannot connect to MQTT.");
        return false;
    }

    const int maxAttempts = 3;
    int attempt = 1;

    while (attempt <= maxAttempts) {
        client_id = "esp8266-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT Broker as %s (Attempt %d)...\n", client_id.c_str(), attempt);

        if (mqtt_client.connect(client_id.c_str())) {
            Serial.println("Connected to MQTT broker");

            String response_topic = String(mqtt_base_topic) +
                                   "/parking/entry/" +
                                   String(DEVICE_ID) +
                                   "/response";

            if (mqtt_client.subscribe(response_topic.c_str())) {
                return true;
            } else {
                Serial.println("Subscription failed");
                mqtt_client.disconnect();
            }
        } else {
            Serial.printf("Failed (rc=%d)\n", mqtt_client.state());
        }

        attempt++;
        if (attempt <= maxAttempts) {
            Serial.println("Retrying in 5 seconds...");
            delay(5000);
        }
    }

    Serial.println("Could not connect to MQTT broker after 3 attempts.");
    return false;
}

void MQTTService::loop() {
    if (!mqtt_client.connected()) {
        Serial.println("MQTT disconnected, attempting reconnect...");
        connect();
    }
    mqtt_client.loop();
}

bool MQTTService::publishMessage(const String& topic, const String& message) {
    if (!mqtt_client.connected()) {
        if (!connect()) {
            Serial.println("Cannot publish - MQTT not connected");
            return false;
        }
    }

    bool success = mqtt_client.publish(topic.c_str(), message.c_str());
    if (!success) {
        Serial.println("Publish failed");
    }
    return success;
}

void MQTTService::requestSlot() {
    String topic = String(mqtt_base_topic) + "/parking/entry";
    publishMessage(topic, String(DEVICE_ID));
}

void MQTTService::sendExit(const String& data) {
    String topic = String(mqtt_base_topic) + "/parking/exit";
    publishMessage(topic, data);
}

bool MQTTService::awaitResponse(unsigned long timeout) {
    unsigned long startTime = millis();

    while (millis() - startTime < timeout) {
        loop();

        if (slot != "null") {
            return true;
        }

        delay(100);
    }
    return false;
}