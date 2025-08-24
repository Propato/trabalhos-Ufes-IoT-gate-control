#ifndef SERVOMANAGER_H
#define SERVOMANAGER_H

#include <Servo.h>

#define SERVO_PIN D3
class ServoManager {
public:
    void setup();
    void open();
    void close();
private:
    Servo s1;
};
#endif
