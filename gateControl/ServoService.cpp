#include "ServoService.h"

void ServoService::setup() {
    s1.attach(SERVO_PIN);
    close();
}

void ServoService::open() {
    s1.write(180);
    delay(3000);
}

void ServoService::close() {
    s1.write(0);
    delay(500);
}
