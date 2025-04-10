#include "hold_cans.h"

hold_cans::hold_cans()
    : servo1(SERVO1_PIN, MAX_ANGLE + 12, 30), // grabAngle, releaseAngle
      servo2(SERVO2_PIN, 4, MAX_ANGLE-4),
      servo3(SERVO3_PIN, MAX_ANGLE, 14),
      servo4(SERVO4_PIN, 10, MAX_ANGLE - 1) {}

void hold_cans::begin() {
    pwmServo1.attach(SERVO1_PIN);
    pwmServo2.attach(SERVO2_PIN);
    pwmServo3.attach(SERVO3_PIN);
    pwmServo4.attach(SERVO4_PIN);

    releaseAll();
}

void hold_cans::grabCenter() {
    servo2.grab();
    servo3.grab();
}

void hold_cans::releaseCenter() {
    servo2.release();
    servo3.release();
}

void hold_cans::grabExternal() {
    servo1.grab();
    servo4.grab();
}

void hold_cans::releaseExternal() {
    servo1.release();
    servo4.release();
}

void hold_cans::grabAll() {
    grabCenter();
    //delay(200);
    grabExternal();
}

void hold_cans::releaseAll() {
    releaseCenter();
    //delay(200);
    releaseExternal();
}

void hold_cans::grabNumber(int number) {
    switch (number) {
    case 1:
        servo1.grab();
        break;
    case 2:
        servo2.grab();
        break;
    case 3:
        servo3.grab();
        break;
    case 4:
        servo4.grab();
        break;
    case 5:
        grabCenter();
        break;
    case 6:
        releaseCenter();
        break;
    case 7:
        grabExternal();
        break;
    case 8:
        releaseExternal();
        break;
    case 9:
        grabAll();
        break;
    case 10:
        releaseAll();
        break;
    default:
        break;
    }
}

void hold_cans::releaseNumber(int number) {
    switch (number) {
    case 1:
        servo1.release();
        break;
    case 2:
        servo2.release();
        break;
    case 3:
        servo3.release();
        break;
    case 4:
        servo4.release();
        break;
    default:
        break;
    }
}
