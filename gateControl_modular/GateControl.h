#ifndef GATECONTROL_H
#define GATECONTROL_H

#include "WiFiManager.h"
#include "MQTTManager.h"
#include "RFIDManager.h"
#include "ButtonManager.h"
#include "ServoManager.h"

class GateControl {
public:
    void setup();
    void loop();
private:
    enum State {
        IDLE,
        WAIT_CARD_WRITE,
        WRITE_CARD,
        WAIT_CARD_READ,
        READ_CARD,
        OPEN_GATE,
        CLOSE_GATE
    };

    State currentState = IDLE;

    WiFiManager wifi;
    MQTTManager mqtt;
    RFIDManager rfid;
    ButtonManager buttons;
    ServoManager servo;

    void handleState();
};

#endif
