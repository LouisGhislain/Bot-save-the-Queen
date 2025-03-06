#include "servo_class_v1.h"
#include <PWMServo.h>

ServoMotor::ServoMotor(int pin, int grabAngle, int releaseAngle) {
    this->pin = pin;
    this->grabAngle = grabAngle;
    this->releaseAngle = releaseAngle;
    pinMode(pin, OUTPUT); 
}

void ServoMotor::moveToAngle(int angle) {
    /*int pulseWidth = map(angle, 0, 270, 500, 2500);  // Convert angle to pulse width
    for (int i = 0; i < 5; i++) {  // Maintain 50Hz signal for proper movement (enough time to reach the target)
        digitalWrite(pin, HIGH);
        delayMicroseconds(pulseWidth);
        digitalWrite(pin, LOW);
        delayMicroseconds(20000 - pulseWidth);  // Maintain 20ms period = 1/50Hz
    }*/
    int analogValue = map(angle, 0, 270, 6, 32);
    analogWrite(pin, analogValue);
}

void ServoMotor::grab() {
    moveToAngle(grabAngle);
}

void ServoMotor::release() {
    moveToAngle(releaseAngle);
}
