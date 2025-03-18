#include "../../include/Robot.h"
#define STATE1_PIN 6
#define STATE2_PIN 19

void Robot::teensy_cans(){  
    pinMode(STATE1_PIN, OUTPUT);
    pinMode(STATE2_PIN, OUTPUT);

    //Sending 01 on the teensy
    digitalWrite(STATE1_PIN, HIGH);
    digitalWrite(STATE2_PIN, LOW);
    delay(10000);
    //Turn off
    digitalWrite(STATE1_PIN, LOW);
    std::cout << "Stop sending" << std::endl ;
}
