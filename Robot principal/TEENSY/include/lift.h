#ifndef LIFT_H
#define LIFT_H

#include <AccelStepper.h>
#include <Arduino.h>

// Define stepper motor connections
#define STEP_PIN_LIFT 35
#define DIR_PIN_LIFT 34
#define EN_PIN_LIFT 40
#define MS1 39 // Microstepping
#define MS2 38
#define MS3 37
#define MICROSWTICH_LIFT_PIN 27

#define zero_level_offset 83.5 // 83.8 mm = hauteur du microswitch par rapport au bas du robot

// 1.8°/step -> 360° = 200 step 
// but microstepping exists 1/8 microstepping --> 1600 steps = 1 rev 

// pulley diameter = 22mm = 2.2cm
// circumference = 2pi*1.1 cm = 6.9 cm

// Move to desired height 
//-> 1rev = 1600 steps 
//-> 1rev = 2pi*r = 2pi*11mm = 69.115 mm 
//=> 1mm = 1600/69.115 = 23.15 steps 
//take microswitch offset into account

class lift {
public:
    lift();
    void begin();
    void moveToHome();
    void up_and_down(int height_in_mm);
    void calibration();

private:
    AccelStepper stepper;
};

#endif // LIFT_H
