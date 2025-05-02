#pragma once

#include <Arduino.h>

class Sonar {
public:
    Sonar(int pin_trig, int pin_echo);      
    int Sonar_Get_Distance();               

private:
    int PIN_TRIG;
    int PIN_ECHO;
};


//#define Sonar_trig A3
//#define Sonar_echo 4