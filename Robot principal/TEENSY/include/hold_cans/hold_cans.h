#ifndef HOLD_CANS_H
#define HOLD_CANS_H

#include "Arduino.h"
#include "servo_motor.h"
#include <PWMServo.h>
#include <string.h> //needed for memcpy


// Define servo pins
#define SERVO1_PIN 28
#define SERVO2_PIN 29
#define SERVO3_PIN 33
#define SERVO4_PIN 36

#define MAX_ANGLE 60

class hold_cans {
public:
    hold_cans();
    void begin();
    void grabCenter();
    void releaseCenter();
    void grabExternal();
    void releaseExternal();
    void grabAll();
    void releaseAll();
    void grabNumber(int number);
    void releaseNumber(int number);

private:
    ServoMotor servo1;
    ServoMotor servo2;
    ServoMotor servo3;
    ServoMotor servo4;
    PWMServo pwmServo1, pwmServo2, pwmServo3, pwmServo4;
};

#endif // SERVO_CONTROLLER_H
