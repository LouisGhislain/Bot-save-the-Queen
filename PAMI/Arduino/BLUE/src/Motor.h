#pragma once
#include <Arduino.h>
#include "Encoder.h"

class Motor {
private:
    int PWM_PIN;
    int dir1; // Pin de direction 1
    int dir2; // Pin de direction 2
    int PWM_max = 255;
    int tension_max = 9;
    double speed;
    double distance; // Distance parcourue par le moteur
    double targetSpeed; // Vitesse cible du moteur
    int TICKS_PER_REV = 13;

    Encoder* encoder; // Lien vers l'encodeur associé

    // Time interval in seconds
    double lastTime;
    double currentTime;

    int lastTicks;
    int currentTicks;

    static Motor* leftInstance;
    static Motor* rightInstance;


public:

    Motor(int pwmPin, int DIR1, int DIR2, Encoder* encoder, bool isLeft = false);
    void updateSpeed();

    void brake();
    void stop_motor();
    void set_motor(float tension);
    double getSpeed();
    void updateDistance();
    double getDistance();
    void resetDistance();
    void reset();


};