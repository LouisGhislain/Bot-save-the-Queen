#include <Arduino.h>
#include "Microswitch.h"


Microswitch* Microswitch::starting_switch = nullptr;
Microswitch* Microswitch::left_switch = nullptr;
Microswitch* Microswitch::right_switch = nullptr;

Microswitch::Microswitch(int pin) : PIN(pin){
    pinMode(PIN, INPUT);
}

// Fonction pour lire l'état du switch
void Microswitch::Check_press() {
    // If switch is pressed (High state) print switch is pressed
    int switchState = 0 ; 
    switchState = digitalRead(PIN);
    if (switchState == HIGH) {
        Serial.println("Switch is pressed.");
    }
    if (switchState == LOW) {
        Serial.println("Switch is not pressed.");
    }
}

int Microswitch::switch_state() {
    // If switch is pressed (High state) print switch is pressed
    int switchState = 0 ; 
    switchState = digitalRead(PIN);
    return switchState;
}
