// Include guard
#pragma once

// Include the necessary libraries
#include <Arduino.h>
#define Sonar_trig A3
#define Sonar_echo 4

//extern volatile bool state_asking;
void Pin_Sonar_Initialization();
int Sonar_Get_Distance();
