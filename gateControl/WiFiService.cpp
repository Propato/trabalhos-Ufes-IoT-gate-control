#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "WiFiService.h"

bool WiFiService::setup() {
    WiFi.begin(ssid, password);
    int count = 0;
    connected = true;

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        count++;
        if (count > 60) {  // Timeout after 30s
            Serial.println("\nFailed to connect to WiFi");
            connected = false;
            return false;
        }
    }

    Serial.println("\nWiFi connected!");
    return true;
}
