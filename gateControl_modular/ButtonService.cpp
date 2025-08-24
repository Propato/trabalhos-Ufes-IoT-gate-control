#include "ButtonService.h"

void ButtonService::setup() {
    pinMode(BUTTON_WRITE, INPUT_PULLUP);
    pinMode(BUTTON_READ, INPUT_PULLUP);
}

void ButtonService::update() {
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

bool ButtonService::isWritePressed() {
    return stableWriteState == LOW;
}

bool ButtonService::isReadPressed() {
    return stableReadState == LOW;
}
