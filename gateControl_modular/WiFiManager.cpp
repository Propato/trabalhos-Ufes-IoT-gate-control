#include "WiFiManager.h"
const char *ssid = "Marcia";
const char *password = "504617cm";

void WiFiManager::setup() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}
