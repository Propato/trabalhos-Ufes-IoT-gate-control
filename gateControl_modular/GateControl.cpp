#include "GateControl.h"

void GateControl::setup() {
    wifi.setup();
    mqtt.setup();
    rfid.setup();
    buttons.setup();
    servo.setup();
}

void GateControl::loop() {
    buttons.update();
    handleState();
    mqtt.loop();
}

void GateControl::handleState() {
    switch (currentState) {
        case IDLE:
            if (buttons.isWritePressed()) {
                currentState = WAIT_CARD_WRITE;
            } else if (buttons.isReadPressed()) {
                currentState = WAIT_CARD_READ;
            }
            break;
        case WAIT_CARD_WRITE:
            if (rfid.isCardPresent()) {
                mqtt.requestSlot();
                currentState = WRITE_CARD;
            }
            break;
        case WRITE_CARD:
            if (mqtt.hasSlot()) {
                cardData = mqtt.getSlot();
                rfid.writeCard(cardData);
                currentState = OPEN_GATE;
            }
            break;
        case WAIT_CARD_READ:
            if (rfid.isCardPresent()) {
                cardData = rfid.readCard();
                mqtt.sendExit(cardData);
                currentState = OPEN_GATE;
            }
            break;
        case OPEN_GATE:
            servo.open();
            currentState = CLOSE_GATE;
            break;
        case CLOSE_GATE:
            servo.close();
            currentState = IDLE;
            break;
    }
}
