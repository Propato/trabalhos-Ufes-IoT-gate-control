#ifndef GATECONTROL_H
#define GATECONTROL_H

#include "RFIDManager.h"
#include "ButtonManager.h"
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "ServoManager.h"

class GateControl {
public:
    void setup();
    void loop();
private:
    enum State {
        IDLE,
        WAIT_CARD_WRITE,
        WAIT_CARD_READ,
        WRITE_CARD,
        READ_CARD,
        OPEN_GATE,
        CLOSE_GATE
    };
    State currentState = IDLE;
    RFIDManager rfid;
    ButtonManager buttons;
    WiFiManager wifi;
    MQTTManager mqtt;
    ServoManager servo;
    String cardData;
    void handleState();
};

#endif
