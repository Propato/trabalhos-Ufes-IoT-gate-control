#include "GateControl.h"
#include "RFIDService.h"
#include "ButtonService.h"
#include "WiFiService.h"
#include "MQTTService.h"
#include "ServoService.h"

GateControl gateControl;

void setup() {
    Serial.begin(115200);
    gateControl.setup();
}

void loop() {
    gateControl.loop();
}
