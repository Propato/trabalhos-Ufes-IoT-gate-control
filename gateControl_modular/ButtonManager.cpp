#include "ButtonManager.h"

void ButtonManager::setup() {
    pinMode(BUTTON_WRITE, INPUT_PULLUP);
    pinMode(BUTTON_READ, INPUT_PULLUP);
}

void ButtonManager::update() {
    int readingWrite = digitalRead(BUTTON_WRITE);
    int readingRead  = digitalRead(BUTTON_READ);

    unsigned long currentTime = millis();

    // Writing Debounce
    if (readingWrite != lastWriteState) {
        lastWriteDebounceTime = currentTime;
    }
    if ((currentTime - lastWriteDebounceTime) > debounceDelay) {
        stableWriteState = readingWrite;
    }
    lastWriteState = readingWrite;

    // Reading Debounce
    if (readingRead != lastReadState) {
        lastReadDebounceTime = currentTime;
    }
    if ((currentTime - lastReadDebounceTime) > debounceDelay) {
        stableReadState = readingRead;
    }
    lastReadState = readingRead;
}

bool ButtonManager::isWritePressed() {
    return stableWriteState == LOW;
}

bool ButtonManager::isReadPressed() {
    return stableReadState == LOW;
}
