#ifndef MOTEURSERVO_H
#define MOTEURSERVO_H

#include <Arduino.h>
#include <AccelStepper.h>
#include <Servo.h>

// Déclaration des broches moteur pas à pas
#define STEP_PIN 14
#define DIR_PIN 41
#define ENABLE_PIN 20  

// Déclaration des broches servos
#define SERVO_PIN 9
#define SERVO_PIN2 10

// Déclaration des microswitch 
#define SWITCH_RIGHT 26 
#define SWITCH_LEFT 11  

// Déclaration des objets moteur et servos
extern AccelStepper stepper;
extern Servo myservo1;
extern Servo myservo2;



// Fonctions
void Stepper(int position);
void servo_motor_right(int angle);
void servo_motor_left(int angle);

void calibration(); 
void routine_separation_stack ();

#endif
