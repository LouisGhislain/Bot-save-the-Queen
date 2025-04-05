#include <Arduino.h>
#include <Servo.h>
#include "Motor.h"
#include "Encoder.h"
#include "Tail.h"
#include "Microswitch.h"
#include "Sonar.h"


volatile int leftTicks_data = 0;
volatile int rightTicks_data = 0;
const float TIME_INTERVAL = 0.0005; // 500µs en secondes
float Interval = 0;
unsigned long startTime;
int iteration = 0;

//TRUC DE CHACHAT
unsigned long previousStepTime = 0;
int motionStep = 0;  // Pour suivre quelle étape on est en train d'exécuter
bool isStopped = false;


void setup() {
    Serial.begin(9600);
    Serial.println("Attente de 5 secondes...");
    delay(100); // Attente avant le démarrage du test

    Pin_Motor_Initialization();
    Pin_Sonar_Initialization();
    
}
  
  void loop() {
      // Vérifie la distance en permanence
    double distance = Sonar_Get_Distance();
    delay(60);
    //Serial.print("Distance : ");
    //Serial.print(distance, 2);  // 2 décimales
    //Serial.println(" cm");


      if (distance < 8.0) {
          active_brake();
          Serial.print("JE FREINE");
          isStopped = true;
          return;  // On ne fait rien d'autre si obstacle
      } else {
          isStopped = false;
      }
  
      // S'il n'y a pas d'obstacle, exécuter la séquence de mouvement sans delay bloquant
      unsigned long currentTime = millis();
  
      if (currentTime - previousStepTime > 20 && !isStopped && distance!=0) {
          switch (motionStep) {
              case 0:
                  set_motor(4, 5);  // Avance
                  previousStepTime = currentTime;
                  motionStep++;
                  break;
              case 1:
                  set_motor(3, 3);  // Tourne un peu
                  previousStepTime = currentTime;
                  motionStep++;
                  break;
              case 2:
                  set_motor(4, 5);  // Reprend l'avance
                  previousStepTime = currentTime;
                  motionStep = 0;  // Reboucle
                  break;
          }
      }
  }
  


    
    
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
