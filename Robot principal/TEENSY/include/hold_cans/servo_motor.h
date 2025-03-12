#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H
class ServoMotor {
public:
    ServoMotor(int pin, int grabAngle, int releaseAngle);
    void moveToAngle(int angle);
    void grab();
    void release();

private:
    int pin;
    int grabAngle;
    int releaseAngle;
};

#endif // SERVO_MOTOR_H
