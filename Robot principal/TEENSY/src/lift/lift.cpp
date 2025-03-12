#include "lift.h"
#include <Arduino.h>


// Constructor: Initialize stepper object
lift::lift() : stepper(1, STEP_PIN, DIR_PIN) {}

void lift::begin() {
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(EN_PIN, OUTPUT);
    pinMode(MICROSWTICH_LIFT_PIN, INPUT);

    // Enable the driver
    digitalWrite(EN_PIN, LOW);  // Enable TMC2209 (LOW to enable)

    // Configure stepper motor
    stepper.setMaxSpeed(2000);
    stepper.setAcceleration(3000);
}

void lift::moveToHome() {
    stepper.moveTo(10000);  // Large movement towards home position
    while (digitalRead(MICROSWTICH_LIFT_PIN) == 1) {  // While switch is not pressed
        stepper.run();  // Move stepper
    }
}

void lift::up_and_down(int height_in_mm) {
    //Serial.println(digitalRead(MICROSWTICH_LIFT_PIN));

    stepper.moveTo(-height_in_mm*23);  // Move 1 full revolution (assuming 1/16 microstepping) //environ 5000 pas = montée jusqu'en haut 
    while (stepper.distanceToGo() != 0) {
      stepper.run();  // Must be called continuously
    }

    //delay(2000);  // Wait

    /*stepper.moveTo(0);  // Move back to start
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }

    delay(2000);  // Wait*/
}
