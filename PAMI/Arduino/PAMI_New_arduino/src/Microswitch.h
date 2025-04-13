// Include guard
#pragma once

// Include the necessary libraries
#include <Arduino.h>

class Microswitch{
public : 
    Microswitch(int pin);

    void Check_press(); // Fonction pour lire l'état du switch

    int switch_state();
private : 
    int PIN ;
    static Microswitch* starting_switch;
    static Microswitch* left_switch;
    static Microswitch* right_switch; 
};
