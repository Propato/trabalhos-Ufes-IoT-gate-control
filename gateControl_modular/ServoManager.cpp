#include "ServoManager.h"

void ServoManager::setup() {
    s1.attach(SERVO_PIN);
    close();
}

void ServoManager::open() {
    s1.write(180);
    delay(3000);
}

void ServoManager::close() {
    s1.write(0);
    delay(500);
}
