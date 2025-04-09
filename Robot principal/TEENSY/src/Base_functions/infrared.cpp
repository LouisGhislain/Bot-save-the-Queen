#include "infrared.h"

float give_distance(){
    int rightValue = analogRead(PIN_IR_RIGHT);
    int leftValue = analogRead(PIN_IR_LEFT);
    int sensorValue = (rightValue + leftValue) / 2;

    // Conversion en cm basée sur les caractéristiques du GP2Y0A41SK0F
    float voltage = sensorValue * (5.0 / 1023.0);  // Convertir en tension (5V max)
    
    // Approximation de la distance en cm selon la courbe de réponse du capteur
    //float distance = 13.0 / (voltage - 0.15);  // Formule empirique basée sur la doc
    float distance = (13.0 / (voltage)) -1.1 ; 
    /*
    if(distance>30){
        Serial.println("Teensy - infrared- trop loin (supérieur à 30cm)");
    }
    */

    return distance;
}