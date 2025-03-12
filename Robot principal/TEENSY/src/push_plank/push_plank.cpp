
#include <Arduino.h>
#include "push_plank.h"

// Initialisation du moteur pas à pas
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Initialisation des servos
Servo myservo1;
Servo myservo2;

void Stepper(int position) {
    stepper.setMaxSpeed(1500);
    stepper.setAcceleration(1000);
    stepper.setSpeed(500);

    stepper.moveTo(position);
    while (stepper.distanceToGo() != 0) {
        stepper.run();
    }
}


void servo_motor_left(int angle) {
    // Position haute = 175
    // Position basse = 70
    myservo1.attach(SERVO_PIN);
    myservo1.write(angle);
}

void servo_motor_right(int angle) {
    // Position haute = 210  => Problème car monte pas assez haut 
    // Position basse = 90
    myservo2.attach(SERVO_PIN2);
    myservo2.write(angle);
}


void calibration() {
    stepper.setMaxSpeed(500);
    stepper.setAcceleration(300);
    stepper.setSpeed(200);

    // Avancer jusqu'à ce que le microswitch soit activé   && digitalRead(SWITCH_RIGHT) == HIGH
    while (digitalRead(SWITCH_RIGHT) == LOW ) {
        stepper.move(10);
        stepper.run();
    }

    // Reculer légèrement pour libérer le switch
    stepper.move(-10);
    while (stepper.distanceToGo() != 0) {
        stepper.run();
    }

    // Définir cette position comme zéro
    stepper.setCurrentPosition(0);
}

void routine_separation_stack (){
    delay(3000);

    // Position basse
    servo_motor_right(103);
    servo_motor_left(83);
    delay(1000);

    Stepper(-3000);
    delay(1000);

    Stepper(0);
    delay(1000);

    // Position haute 
    servo_motor_right(210); 
    servo_motor_left(175);
    delay(1000);
}

