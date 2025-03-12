#ifndef LIFT_H
#define LIFT_H

#include <Arduino.h>
#include <AccelStepper.h>
#include <string.h> //needed for memcpy


// Define stepper motor connections
#define STEP_PIN 35
#define DIR_PIN 34
#define EN_PIN 40
#define MS1 39 // Microstepping
#define MS2 38
#define MS3 37
#define MICROSWTICH_LIFT_PIN 27

// 1.8°/step -> 360° = 200 step 
// but microstepping exists 1/8 microstepping --> 1600 steps = 1 rev 

// pulley diameter = 22mm = 2.2cm
// circumference = 2pi*1.1 cm = 6.9 cm

// 1 rev = 1600 steps -> 6.9 cm 
// 1 cm ->  231.88 steps ~= 232 steps
// 1 mm -> 23 steps

class lift {
public:
    lift();
    void begin();
    void moveToHome();
    void up_and_down(int height_in_mm);

private:
    AccelStepper stepper;
};

#endif // LIFT_H
