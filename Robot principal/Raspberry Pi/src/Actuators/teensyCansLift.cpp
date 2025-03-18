#include "../../include/Robot.h"
#define STATE1_PIN 6
#define STATE2_PIN 19

void Robot::teensy_cans_lift(){  
    pinMode(STATE1_PIN, OUTPUT);
    pinMode(STATE2_PIN, OUTPUT);
    
    //Sending 11 on the teensy
    digitalWrite(STATE1_PIN, HIGH);
    digitalWrite(STATE2_PIN, HIGH);
    delay(5000);
    //Turn off
    digitalWrite(STATE1_PIN, LOW);
    digitalWrite(STATE2_PIN, LOW);
    std::cout << "Stop sending" << std::endl ;
}
