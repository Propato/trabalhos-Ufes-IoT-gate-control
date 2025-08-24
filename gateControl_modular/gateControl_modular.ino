#include "GateControl.h"
#include "RFIDManager.h"
#include "ButtonManager.h"
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "ServoManager.h"

GateControl gateControl;

void setup() {
    Serial.begin(115200);
    gateControl.setup();
}

void loop() {
    gateControl.loop();
}
