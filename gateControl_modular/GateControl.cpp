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
            if (rfid.authenticate()) {
                mqtt.requestSlot();
                mqtt.awaitResponse();
                currentState = WRITE_CARD;
            }
            break;
        case WRITE_CARD:
            String slot = mqtt.getSlot();
            if (rfid.writeStringToCard(slot)) {
                mqtt.resetSlot();
                currentState = OPEN_GATE;
            } else {
                currentState = WAIT_CARD_WRITE;
            }
            break;

        case WAIT_CARD_READ:
            if (rfid.authenticate()) {
                currentState = READ_CARD;
            }
            break;
        case READ_CARD:
            String slot = rfid.readStringFromCard();
            if (slot != "Reading failed") {
                mqtt.sendExit(slot);
                currentState = OPEN_GATE;
            } else {
                currentState = WAIT_CARD_READ;
            }

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
