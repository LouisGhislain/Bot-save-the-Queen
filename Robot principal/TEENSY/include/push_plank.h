#ifndef MOTEURSERVO_H
#define MOTEURSERVO_H

#include <Arduino.h>
#include <AccelStepper.h>
#include <Servo.h>

// Déclaration des broches moteur pas à pas
#define STEP_PIN_PUSH 14
#define DIR_PIN_PUSH 41
#define EN_PIN_PUSH 20  

// Déclaration des broches servos
#define SERVO_PIN 9
#define SERVO_PIN2 10

// Déclaration des microswitch 
#define SWITCH_RIGHT A12
#define SWITCH_LEFT 11  
class push_plank {
    public:
        push_plank();
        void begin();
        void calibration();
        void move_to_mm(int distance_mm);
        void servo_motor_left(int angle);
        void servo_motor_right(int angle);
        void pull_plank_grab();
        void pull_plank_release();

        void routine_separation_stack();
    
    private:
        AccelStepper stepper;
        Servo myservo1;
        Servo myservo2;
};
#endif
