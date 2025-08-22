#ifndef SERVOMANAGER_H
#define SERVOMANAGER_H
#include <Servo.h>
class ServoManager {
public:
    void setup();
    void open();
    void close();
private:
    Servo s1;
};
#endif
