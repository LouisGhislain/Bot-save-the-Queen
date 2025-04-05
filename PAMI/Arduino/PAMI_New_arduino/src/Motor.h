// Include guard
#pragma once

// Include the necessary libraries
#include <Arduino.h>

// Fonction de configuration des broches
void Pin_Motor_Initialization();

// Fonction de contrôle des moteurs
void set_motor(float tension_left, float tension_right);

//Fonction freinage
void active_brake();