#pragma once

#include <Arduino.h>
#include <Servo.h>

class Tail{
private : 
    int PIN_TAIL ; 
    Servo Servo_Tail;
    static Tail* tail ; 
public : 
    Tail(int pin_tail);
    void Turn_tail();
    void Turn_tail_start(int angle);
};

