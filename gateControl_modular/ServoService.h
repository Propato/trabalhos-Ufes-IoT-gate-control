#ifndef SERVOSERVICE_H
#define SERVOSERVICE_H

#include <Servo.h>

#define SERVO_PIN D3
class ServoService {
public:
    void setup();
    void open();
    void close();
private:
    Servo s1;
};
#endif
