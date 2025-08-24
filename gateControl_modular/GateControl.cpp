#include "GateControl.h"

void GateControl::setup() {
    wifi.setup();
    mqtt.setup();
    rfid.setup();
    buttons.setup();
    servo.setup();

    Serial.println("\nState: IDLE");
}

void GateControl::loop() {
    buttons.update();
    handleState();
    mqtt.loop();
}

void GateControl::handleState() {
    String slot = "";
    static unsigned long responseWaitStart = 0;

    switch (currentState) {
        case IDLE:
            if(lastState != currentState)
                Serial.println("State: IDLE");
            lastState = currentState;

            if (buttons.isWritePressed()) {
                currentState = WAIT_CARD_WRITE;
            } else if (buttons.isReadPressed()) {
                currentState = WAIT_CARD_READ;
            }
            break;

        case WAIT_CARD_WRITE:
            if(lastState != currentState)
                Serial.println("State: WAIT_CARD_WRITE");
            lastState = currentState;

            if (rfid.authenticate()) {
                mqtt.requestSlot();
                responseWaitStart = millis();
                currentState = WRITE_CARD;
            }
            break;
        case WRITE_CARD:
            if(lastState != currentState)
                Serial.println("State: WRITE_CARD");
            lastState = currentState;

            if (mqtt.getSlot() != "null") {
                slot = mqtt.getSlot();
                if (rfid.writeStringToCard(slot)) {
                    mqtt.resetSlot();
                    currentState = OPEN_GATE;
                } else {
                    currentState = WAIT_CARD_WRITE;
                }
            } else if (millis() - responseWaitStart > 10000) {
                Serial.println("Timeout waiting for slot response");
                currentState = IDLE;
            }
            break;

        case WAIT_CARD_READ:
            if(lastState != currentState)
                Serial.println("State: WAIT_CARD_READ");
            lastState = currentState;

            if (rfid.authenticate()) {
                currentState = READ_CARD;
            }
            break;
        case READ_CARD:
            if(lastState != currentState)
                Serial.println("State: READ_CARD");
            lastState = currentState;

            slot = rfid.readStringFromCard();
            if (slot != "Reading failed") {
                mqtt.sendExit(slot);
                currentState = OPEN_GATE;
            } else {
                currentState = WAIT_CARD_READ;
            }

        case OPEN_GATE:
            if(lastState != currentState)
                Serial.println("State: OPEN_GATE");
            lastState = currentState;

            servo.open();
            currentState = CLOSE_GATE;
            break;
        case CLOSE_GATE:
            if(lastState != currentState)
                Serial.println("State: CLOSE_GATE");
            lastState = currentState;

            servo.close();
            currentState = IDLE;
            break;
    }
}
