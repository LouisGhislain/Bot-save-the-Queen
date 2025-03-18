#include "lift.h"


// Constructor: Initialize stepper object
lift::lift() : stepper(1, STEP_PIN_LIFT, DIR_PIN_LIFT) {}

void lift::begin() {
    pinMode(STEP_PIN_LIFT, OUTPUT);
    pinMode(DIR_PIN_LIFT, OUTPUT);
    pinMode(EN_PIN_LIFT, OUTPUT);
    pinMode(MICROSWTICH_LIFT_PIN, INPUT);

    // Enable the driver
    digitalWrite(EN_PIN_LIFT, LOW);  // Enable TMC2209 (LOW to enable)

    // Configure stepper motor
    stepper.setMaxSpeed(2000);
    stepper.setAcceleration(2000);

    //calibration();
}

void lift::moveToHome() {
    stepper.moveTo(-10000);  // Large movement towards home position
    while (digitalRead(MICROSWTICH_LIFT_PIN) == 1) {  // While switch is not pressed
        stepper.run();  // Move stepper
    }
}

void lift::up_and_down(int height_in_mm) {   //height defined as the top of the hold cans
    //Serial.println(digitalRead(MICROSWTICH_LIFT_PIN));

    stepper.moveTo((-height_in_mm + zero_level_offset) * 23.15);  // Move to desired height -> 1rev = 1600 steps -> 1rev = 2pi*r = 2pi*11mm = 69.115 mm => 1mm = 1600/69.115 = 23.15 steps 
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

void lift::calibration() {
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(300);
  stepper.setSpeed(200);

  stepper.move(500);
  while (stepper.distanceToGo() != 0) {
      stepper.run();
  }

  while (digitalRead(MICROSWTICH_LIFT_PIN) == HIGH) {
      stepper.move(-10);
      stepper.run();
  }

  stepper.move(-10);
  while (stepper.distanceToGo() != 0) {
      stepper.run();
  }

  stepper.setCurrentPosition(0);
}
