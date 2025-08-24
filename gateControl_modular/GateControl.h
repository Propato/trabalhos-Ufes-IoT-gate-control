#ifndef GATECONTROL_H
#define GATECONTROL_H

#include "WiFiService.h"
#include "MQTTService.h"
#include "RFIDService.h"
#include "ButtonService.h"
#include "ServoService.h"

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

    WiFiService wifi;
    MQTTService mqtt;
    RFIDService rfid;
    ButtonService buttons;
    ServoService servo;

    void handleState();
};

#endif
