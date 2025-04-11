#include <Arduino.h>
#include <Servo.h>
#include "Motor.h"
#include "Encoder.h"
#include "Tail.h"
#include "Microswitch.h"
#include "Sonar.h"
#include "Pami.h"

// // Pin definitions
// #define ENA 5
// #define ENB 6
// #define DIR2 8
// #define DIR1 7
// #define DIR4 11
// #define DIR3 10


// const float TIME_INTERVAL = 0.0005; // 500µs en secondes
// float Interval = 0;
// unsigned long startTime;
// unsigned long lastMeasureTime;
// int iteration = 0;

// //TRUC DE CHACHAT
// unsigned long previousStepTime = 0;
// int motionStep = 0;  // Pour suivre quelle étape on est en train d'exécuter
// bool isStopped = false;


// // Initialisation des encodeurs
// Encoder leftEncoder(2, A4, true);
// Encoder rightEncoder(3, A0, false);

// // Initialisation des moteurs
// Motor leftMotor(ENA, DIR1, DIR2, &leftEncoder, true);
// Motor rightMotor(ENB, DIR3, DIR4, &rightEncoder, false);

//Initialisation de PAMI
PAMI pami;
float startTime = 0;



void setup() {
    Serial.begin(9600);

    //Initialisation du PAMI
    pami = PAMI();

    Serial.println("Début du test de vitesse");

    delay(5000);  // Attendre que le moniteur série soit prêt

    startTime = millis();

    // En-tête CSV
    // Serial.println("time_ms,left_ticks,left_speed,right_ticks,right_speed");
}

void loop(){

    pami.middlecontrol(1.5, -0.1, 0, false); // Met à jour la position du robot

}

// void loop() {
//     unsigned long now = millis();

//     // Mise à jour des vitesses mesurées
//     leftMotor.updateSpeed();
//     rightMotor.updateSpeed();

//     // --- Motor gauche ---
//     float leftTarget = leftMotor.getTargetSpeed();
//     float leftCurrent = leftMotor.getSpeed();
//     float leftError = leftTarget - leftCurrent;
//     integral_left += leftError * TIME_INTERVAL;
//     float leftCommand = Kp_left * leftError + Ki_left * integral_left;
//     leftMotor.set_motor(leftCommand);

//     // --- Motor droit ---
//     float rightTarget = rightMotor.getTargetSpeed();
//     float rightCurrent = rightMotor.getSpeed();
//     float rightError = rightTarget - rightCurrent;
//     integral_right += rightError * TIME_INTERVAL;
//     float rightCommand = Kp_right * rightError + Ki_right * integral_right;
//     rightMotor.set_motor(rightCommand);

//     // Logging
//     Serial.print(now - startTime); Serial.print(",");
//     Serial.print(leftCurrent); Serial.print(",");
//     Serial.print(leftTarget); Serial.print(",");
//     Serial.print(rightCurrent); Serial.print(",");
//     Serial.println(rightTarget);

//     // Arrêter après 5 secondes
//     if (now - startTime >= 5000) {
//         leftMotor.set_motor(0);
//         rightMotor.set_motor(0);
//         Serial.println("Fin du test.");
//         while (true);
//     }

//     delayMicroseconds(500); // correspond à 500µs
// }









// void setup() {
//     Serial.begin(9600);

//     delay(10000); // Attendre que le moniteur série soit prêt

//     // En-tête CSV
//     Serial.println("time_ms,left_ticks,left_speed,right_ticks,right_speed");

//     startTime = millis();
//     leftMotor.set_motor(7);
//     rightMotor.set_motor(7);
// }

// void loop() {
//     leftMotor.updateSpeed();
//     rightMotor.updateSpeed();

//     // Si test a duré 3 secondes
//     if (millis() - startTime >= 5000) {
//         leftMotor.set_motor(0);
//         rightMotor.set_motor(0);
//         Serial.println("Test terminé.");
//         while (true); // Arrête le programme
//     }
//     else {
//         if (lastMeasureTime == 0 || millis() - lastMeasureTime >= TIME_INTERVAL * 1000) {
//             lastMeasureTime = millis();
    
//             // Récupération des données
//             unsigned long time = millis() - startTime;
//             int leftTicks = leftEncoder.getTicks();
//             float leftSpeed = leftMotor.getSpeed();
//             int rightTicks = rightEncoder.getTicks();
//             float rightSpeed = rightMotor.getSpeed();
    
//             // Impression format CSV (pour logger sur PC)
//             Serial.print(time);
//             Serial.print(",");
//             Serial.print(leftTicks);
//             Serial.print(",");
//             Serial.print(leftSpeed);
//             Serial.print(",");
//             Serial.print(rightTicks);
//             Serial.print(",");
//             Serial.println(rightSpeed);
//         }
//     }

// }






  
//   void loop() {
//       // Vérifie la distance en permanence
//     double distance = Sonar_Get_Distance();
//     delay(60);
//     //Serial.print("Distance : ");
//     //Serial.print(distance, 2);  // 2 décimales
//     //Serial.println(" cm");


//       if (distance < 8.0) {
//           active_brake();
//           Serial.print("JE FREINE");
//           isStopped = true;
//           return;  // On ne fait rien d'autre si obstacle
//       } else {
//           isStopped = false;
//       }
  
//       // S'il n'y a pas d'obstacle, exécuter la séquence de mouvement sans delay bloquant
//       unsigned long currentTime = millis();
  
//       if (currentTime - previousStepTime > 20 && !isStopped && distance!=0) {
//           switch (motionStep) {
//               case 0:
//                   set_motor(4, 5);  // Avance
//                   previousStepTime = currentTime;
//                   motionStep++;
//                   break;
//               case 1:
//                   set_motor(3, 3);  // Tourne un peu
//                   previousStepTime = currentTime;
//                   motionStep++;
//                   break;
//               case 2:
//                   set_motor(4, 5);  // Reprend l'avance
//                   previousStepTime = currentTime;
//                   motionStep = 0;  // Reboucle
//                   break;
//           }
//       }
//   }



  


    
    
    // // Si le switch est pressé (état bas)
    // if (switchState == LOW) {
    //     // Stopper les moteurs
    //     analogWrite(ENA, 0);
    //     analogWrite(ENB, 0);
    //     Serial.println("Switch pressé, arrêt des moteurs.");
    //     while (true);
    // }
    
    // Attendre un petit moment avant de refaire la lecture

// void loop() {
//     unsigned long startTime = millis();
//     unsigned long lastMeasureTime = startTime;

//     while (millis() - startTime < 1500) { // 1 seconde
//         if (millis() - lastMeasureTime >= 1) { // Toutes les 500 µs
//             noInterrupts();
//             int leftTickCount = leftTicks;
//             int rightTickCount = rightTicks;
//             leftTicks = 0;
//             rightTicks = 0;
//             interrupts();

//             // Conversion en vitesse angulaire (rad/s)
//             float leftSpeedRad = (leftTickCount * 2.0 * PI) / (TICKS_PER_REV * (millis() - lastMeasureTime));
//             float rightSpeedRad = (rightTickCount * 2.0 * PI) / (TICKS_PER_REV * (millis() - lastMeasureTime));

//             Serial.print("Left: ");
//             Serial.print(leftSpeedRad);
//             Serial.print(" rad/s, Right: ");
//             // Serial.print(" ");
//             Serial.println(rightSpeedRad);
//             //Print le temps
//             // Serial.print("Temps: ");
//             // Serial.println(micros() - lastMeasureTime);
            
//             lastMeasureTime = millis();
//         }
//     }

//     analogWrite(ENA, 0);
//     analogWrite(ENB, 0);

//     Serial.println("Test terminé.");
//     while (true);
// }
