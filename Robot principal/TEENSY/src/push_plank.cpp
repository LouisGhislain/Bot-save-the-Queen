#include "push_plank.h"


#define distance_extension 135 // mm 

#define servo_right_grab_angle 21
#define servo_left_grab_angle 22
#define servo_right_release_angle 114
#define servo_left_release_angle 116

// engrenage à 13 dents
// crémaillère entre 13 dents : 56.55 mm
// => 56.55mm / revolution 
// 1 rev = 1600 steps
// => 1mm = 1600 / 56.55 = 28.3 steps

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

void push_plank::move_to_mm(int distance_mm) {
    stepper.setMaxSpeed(1500);
    stepper.setAcceleration(1000);
    stepper.setSpeed(500);
    stepper.moveTo(distance_mm * 28.3); // get distance in steps
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

void push_plank::pull_plank_grab() {
    servo_motor_right(servo_right_grab_angle);
    servo_motor_left(servo_left_grab_angle);
}

void push_plank::pull_plank_release() {
    servo_motor_right(servo_right_release_angle);
    servo_motor_left(servo_left_release_angle);
}

void push_plank::routine_separation_stack() {
    pull_plank_grab();
    delay(1000);
    move_to_mm(-distance_extension);
    delay(500);
    pull_plank_release();
    move_to_mm(0);
    delay(1000);
}