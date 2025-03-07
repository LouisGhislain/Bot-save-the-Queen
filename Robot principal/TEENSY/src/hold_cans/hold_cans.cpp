#include "hold_cans.h"

hold_cans::hold_cans()
    : servo1(SERVO1_PIN, MAX_ANGLE + 12, 12),
      servo2(SERVO2_PIN, 10, MAX_ANGLE),
      servo3(SERVO3_PIN, MAX_ANGLE, 0),
      servo4(SERVO4_PIN, 10, MAX_ANGLE + 10) {}

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
    delay(200);
    grabExternal();
}

void hold_cans::releaseAll() {
    releaseCenter();
    delay(200);
    releaseExternal();
}
