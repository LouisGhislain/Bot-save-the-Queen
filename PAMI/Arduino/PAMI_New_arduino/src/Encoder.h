// Include guard
#pragma once

// Include the necessary libraries
#include <Arduino.h>

// Fonction de configuration des broches
void Pin_Encoder_Initialization();

// Fonction reinisialisation du compteur de l'encodeur gauche
void resetLeftEncoder();

// Fonction reinisialisation du compteur de l'encodeur droit
void resetRightEncoder();

// Fonction pour obtenir le compteur de l'encodeur gauche
int getLeftEncoder();

// Fonction pour obtenir le compteur de l'encodeur droit
int getRightEncoder();

// Interrupt service routine pour l'encodeur gauche
void leftEncoder();

// Interrupt service routine pour l'encodeur droit
void rightEncoder();

// Fonction pour obtenir le compteur de l'encodeur gauche en radian
float getLeftEncoderRad();

// Fonction pour obtenir le compteur de l'encodeur droit en radian
float getRightEncoderRad();

