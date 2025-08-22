#include "ButtonManager.h"
#include <Arduino.h>
#define BUTTON_WRITE D2
#define BUTTON_READ D4

void ButtonManager::setup() {
    pinMode(BUTTON_WRITE, INPUT_PULLUP);
    pinMode(BUTTON_READ, INPUT_PULLUP);
}

void ButtonManager::update() {
    lastWriteState = digitalRead(BUTTON_WRITE);
    lastReadState = digitalRead(BUTTON_READ);
}

bool ButtonManager::isWritePressed() {
    return lastWriteState == LOW;
}

bool ButtonManager::isReadPressed() {
    return lastReadState == LOW;
}
