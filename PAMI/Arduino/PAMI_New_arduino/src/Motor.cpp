#include <Arduino.h>
#include "Motor.h"



// Constants
int PWM_max = 255;
int tension_max = 12;

// Pin definitions
#define ENA 5
#define ENB 6
#define DIR2 8
#define DIR1 7
#define DIR4 11
#define DIR3 10

// Pin Initialization

void Pin_Motor_Initialization() {
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(DIR1, OUTPUT);
    pinMode(DIR2, OUTPUT);
    pinMode(DIR3, OUTPUT);
    pinMode(DIR4, OUTPUT);
}

void set_motor(float tension_left, float tension_right) {
    // Set the direction of the motors
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, HIGH);
    digitalWrite(DIR3, LOW);
    digitalWrite(DIR4, HIGH);

    if (tension_left > tension_max) {
        tension_left = tension_max;
    }
    if (tension_right > tension_max) {
        tension_right = tension_max;
    }

    // If 2 tension values are positive => move forward
    if (tension_left >= 0 && tension_right >= 0) {
        analogWrite(ENA, tension_left * PWM_max / tension_max);
        analogWrite(ENB, tension_right * PWM_max / tension_max);
    }

    // If 2 tension values are negative => move backward
    if (tension_left < 0 && tension_right < 0) {
        analogWrite(ENA, -tension_left * PWM_max / tension_max);
        analogWrite(ENB, -tension_right * PWM_max / tension_max);
        digitalWrite(DIR1, HIGH);
        digitalWrite(DIR2, LOW);
        digitalWrite(DIR3, HIGH);
        digitalWrite(DIR4, LOW);
    }

    // If left tension is negative and right tension is positive => turn left
    if (tension_left < 0 && tension_right >= 0) {
        analogWrite(ENA, -tension_left * PWM_max / tension_max);
        analogWrite(ENB, tension_right * PWM_max / tension_max);
        digitalWrite(DIR1, HIGH);
        digitalWrite(DIR2, LOW);
    }

    // If left tension is positive and right tension is negative => turn right
    if (tension_left >= 0 && tension_right < 0) {
        analogWrite(ENA, tension_left * PWM_max / tension_max);
        analogWrite(ENB, -tension_right * PWM_max / tension_max);
        digitalWrite(DIR3, HIGH);
        digitalWrite(DIR4, LOW);
    }
    
}

void active_brake(){
    analogWrite(ENA, 9 * PWM_max / tension_max);
    analogWrite(ENB, 9 * PWM_max / tension_max);
    digitalWrite(DIR3, HIGH);
    digitalWrite(DIR4, HIGH);
    digitalWrite(DIR1, HIGH);
    digitalWrite(DIR2, HIGH);
}