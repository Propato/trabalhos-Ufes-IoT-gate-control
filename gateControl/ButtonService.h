#ifndef BUTTONSERVICE_H
#define BUTTONSERVICE_H

#include <Arduino.h>

#define BUTTON_WRITE D2
#define BUTTON_READ D4

class ButtonService {
public:
    void setup();
    void update();
    bool isWritePressed();
    bool isReadPressed();
private:
    int lastWriteState = HIGH;
    int lastReadState = HIGH;

    int stableWriteState = HIGH;
    int stableReadState = HIGH;

    unsigned long lastWriteDebounceTime = 0;
    unsigned long lastReadDebounceTime = 0;

    static const unsigned long debounceDelay = 50; // 50 ms
};
#endif
