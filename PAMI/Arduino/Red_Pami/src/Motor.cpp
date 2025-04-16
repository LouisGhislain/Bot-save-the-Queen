#include <Arduino.h>
#include "Motor.h"
#include "Encoder.h"

#define TICKS_PER_REV 13 // Nombre de ticks par révolution
#define gearRatio 42 // Rapport de réduction
#define WheelDiameter 0.062 // Diamètre de la roue en mètres

Motor* Motor::leftInstance = nullptr;
Motor* Motor::rightInstance = nullptr;

Motor::Motor(int pwmPin, int DIR1, int DIR2, Encoder* encoder, bool isLeft) : PWM_PIN(pwmPin), dir1(DIR1), dir2(DIR2), speed(0), distance(0), encoder(encoder), lastTime(0), currentTime(0), lastTicks(0), currentTicks(0) {
    pinMode(PWM_PIN, OUTPUT);
    if (isLeft) {
        leftInstance = this;
    } else {
        rightInstance = this;
    }
}

void Motor::updateSpeed() {
    if (encoder == nullptr) {
        speed = 0;
        return;
    }

    currentTicks = encoder->getTicks();
    currentTime = millis() / 1000.0; // Convertir en secondes

    float deltaTime = currentTime - lastTime;
    if (deltaTime > 0) {
        speed = (currentTicks - lastTicks) / deltaTime; // Vitesse en ticks/s
        // Speed in rad/s
        speed = (currentTicks - lastTicks) / (2*PI*TICKS_PER_REV * deltaTime*gearRatio);
    } else {
        speed = 0;
    }

    lastTime = currentTime;
    lastTicks = currentTicks;


    updateDistance();
}

void Motor::updateDistance() {
    // Calculer la distance parcourue par le moteur
    distance = encoder->getTicks() * (PI * WheelDiameter) / (TICKS_PER_REV*gearRatio); // Distance en mètres
    
}


void Motor::resetDistance() {
    distance = 0; // Réinitialiser la distance parcourue
}

double Motor::getDistance() {
    return distance; // Distance parcourue en mètres
}

double Motor::getSpeed() {
    return speed; // Vitesse en ticks/s
}


void Motor::set_motor(float tension) {

    // updateSpeed();

    float EMF = speed *15;
    
    if (tension > tension_max) {
        tension = tension_max;
    } else if (tension < -tension_max) {
        tension = -tension_max;
    }


    double dutyCycle = (tension - EMF) / (tension_max - EMF);

    if (dutyCycle > 0) {
        digitalWrite(dir1, LOW);
        digitalWrite(dir2, HIGH);
    } else if (dutyCycle < 0) {
        digitalWrite(dir1, HIGH);
        digitalWrite(dir2, LOW);
    } else {
        stop_motor();
        return;
    }





    // if (tension > 0) {
    //     digitalWrite(dir1, LOW);
    //     digitalWrite(dir2, HIGH);
    // } else if (tension < 0) {
    //     digitalWrite(dir1, HIGH);
    //     digitalWrite(dir2, LOW);
    // } else {
    //     stop_motor();
    //     return;
    // }

    // int pwmValue = abs(tension) * PWM_max / tension_max;
    analogWrite(PWM_PIN, abs(dutyCycle * PWM_max)); // Appliquer la tension au moteur
}


void Motor::stop_motor() {
    analogWrite(PWM_PIN, 0);
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, LOW);
}

void Motor::brake(){
    analogWrite(PWM_PIN,8);
    digitalWrite(dir1,HIGH);
    digitalWrite(dir2,HIGH);
}