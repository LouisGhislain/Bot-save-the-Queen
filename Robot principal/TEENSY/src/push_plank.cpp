#include "push_plank.h"

push_plank::push_plank() : stepper(AccelStepper::DRIVER, STEP_PIN_PUSH, DIR_PIN_PUSH) {}

void push_plank::begin() {
    pinMode(STEP_PIN_PUSH, OUTPUT);
    pinMode(DIR_PIN_PUSH, OUTPUT);
    pinMode(EN_PIN_PUSH, OUTPUT);
    pinMode(SWITCH_RIGHT, INPUT);

    // Enable the driver
    digitalWrite(EN_PIN_PUSH, LOW);  // Enable TMC2209 (LOW to enable)

    // Configure stepper motor
    stepper.setMaxSpeed(1500);
    stepper.setAcceleration(1000);

    calibration();
}

void push_plank::calibration() {
    stepper.setMaxSpeed(500);
    stepper.setAcceleration(300);
    stepper.setSpeed(200);

    while (digitalRead(SWITCH_RIGHT) == LOW) {
        stepper.move(10);
        stepper.run();
    }

    stepper.move(-10);
    while (stepper.distanceToGo() != 0) {
        stepper.run();
    }

    stepper.setCurrentPosition(0);
}

void push_plank::Stepper(int position) {
    stepper.setMaxSpeed(1500);
    stepper.setAcceleration(1000);
    stepper.setSpeed(500);
    stepper.moveTo(position);
    while (stepper.distanceToGo() != 0) {
        stepper.run();
    }
}

void push_plank::servo_motor_left(int angle) {
    myservo1.attach(SERVO_PIN);
    myservo1.write(angle);
}

void push_plank::servo_motor_right(int angle) {
    myservo2.attach(SERVO_PIN2);
    myservo2.write(angle);
}

void push_plank::routine_separation_stack() {
    delay(3000);
    servo_motor_right(103);
    servo_motor_left(83);
    delay(1000);
    Stepper(-3000);
    delay(1000);
    Stepper(0);
    delay(1000);
    servo_motor_right(210);
    servo_motor_left(175);
    delay(1000);
}