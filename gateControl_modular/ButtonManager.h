#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H
class ButtonManager {
public:
    void setup();
    void update();
    bool isWritePressed();
    bool isReadPressed();
private:
    int lastWriteState = HIGH;
    int lastReadState = HIGH;
};
#endif
