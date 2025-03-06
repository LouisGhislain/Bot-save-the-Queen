#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include <Arduino.h>

class ServoMotor {
    private:
        int pin;
        int grabAngle;
        int releaseAngle;
    
    public:
        ServoMotor(int pin, int grabAngle, int releaseAngle);
        void moveToAngle(int angle);
        void grab();
        void release();
};

#endif // SERVO_MOTOR_H