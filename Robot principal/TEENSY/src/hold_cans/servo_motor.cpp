#include "servo_motor.h"
#include <PWMServo.h>

ServoMotor::ServoMotor(int pin, int grabAngle, int releaseAngle)
    : pin(pin), grabAngle(grabAngle), releaseAngle(releaseAngle) {
    pinMode(pin, OUTPUT);
}

void ServoMotor::moveToAngle(int angle) {
    int analogValue = map(angle, 0, 270, 6, 32);
    analogWrite(pin, analogValue);
}

void ServoMotor::grab() {
    moveToAngle(grabAngle);
}

void ServoMotor::release() {
    moveToAngle(releaseAngle);
}
