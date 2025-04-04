#include <Arduino.h>
#include "Microswitch.h"

const int switchPin = 13;  // Pin où le switch est connecté
int switchState = 0;       // Variable pour stocker l'état du switch

// Fonction de configuration des broches
void Pin_Microswitch_Initialization(){
    pinMode(switchPin, INPUT);
}

// Fonction pour lire l'état du switch
void Check_press() {
    // If switch is pressed (High state) print switch is pressed
    switchState = digitalRead(switchPin);
    if (switchState == HIGH) {
        Serial.println("Switch is pressed.");
    }
    if (switchState == LOW) {
        Serial.println("Switch is not pressed.");
    }
}

int Microswitch_state () {
    // If switch is pressed (High state) print switch is pressed
    switchState = digitalRead(switchPin);
    return switchState;
}