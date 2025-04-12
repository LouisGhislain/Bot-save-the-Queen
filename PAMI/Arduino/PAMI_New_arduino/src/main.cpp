#include <Arduino.h>
#include <Servo.h>
#include "Motor.h"
#include "Encoder.h"
#include "Tail.h"
#include "Microswitch.h"
#include "Sonar.h"
#include "Pami.h"
#include "FSM.h"
// #include "Struct.h"

// // Pin definitions
#define ENA 5
#define ENB 6
#define DIR2 8
#define DIR1 7
#define DIR4 11
#define DIR3 10


// const float TIME_INTERVAL = 0.0005; // 500µs en secondes
// float Interval = 0;
// unsigned long startTime;
// unsigned long lastMeasureTime;
// int iteration = 0;



// // Initialisation des encodeurs
Encoder leftEncoder(2, A4, true);
Encoder rightEncoder(3, A0, false);

// // Initialisation des moteurs
Motor leftMotor(ENA, DIR1, DIR2, &leftEncoder, true);
Motor rightMotor(ENB, DIR3, DIR4, &rightEncoder, false);

//Initialisation de PAMI
PAMI pami;
bool isStopped = false ; 
float startTime = 0;

// Robot SuperStar = {STRAIGHT1, 0, false, false, false, false, false};

void setup() {
    Serial.begin(9600);

    // // Initialisation des moteurs
    // leftMotor.set_motor(0);
    // rightMotor.set_motor(0);
    // // Initialisation des encodeurs
    // leftEncoder.reset();
    // rightEncoder.reset();

    //Initialisation du PAMI
    pami = PAMI();

    Serial.println("Début du test de vitesse");

    delay(5000);  // Attendre que le moniteur série soit prêt

    startTime = millis();

    // En-tête CSV
    // Serial.println("time_ms,left_ticks,left_speed,right_ticks,right_speed");
}

void loop(){

    // rightMotor.updateSpeed();
    // rightMotor.set_motor(7);
    // leftMotor.set_motor(7);
    // Serial.print(millis() - startTime);
    // Serial.print(", ");
    // Serial.print(leftMotor.getSpeed());
    // Serial.print(", ");
    // Serial.println(rightMotor.getSpeed());

    // pami.lowlevelcontrol(0.3, 0.3); // Met à jour la vitesse des moteurs
    // pami.middlecontrol(1, 0.8, 0, false); // Met à jour la position du robot
    


    // double distance = pami.getSonarDistance();
    // Serial.print("Distance: ");
    // Serial.println(distance);
    // // delay(60);
    // if (distance < 8.0) {
    //     pami.pami_brake();
    //     Serial.print("JE FREINE");
    //     isStopped = true;
    //     return;  // On ne fait rien d'autre si obstacle
    // } else {
    //    isStopped = false;
    // }
          

    // if (!isStopped && distance!=0) {
    //     pami.middlecontrol(1.5, -0.1, 0, false); // Met à jour la position du robot

    // }
    pami.middlecontrol(1, -1, 0, false); // Met à jour la position du robot

    // switch (SuperStar.state) {
    //     case WAIT:
    //         if (SuperStar.startTime == 0) {
    //             SuperStar.startTime = millis();
    //             Serial.println("Start wait");
    //         }
    //         if (millis() - SuperStar.startTime >= 85000) {
    //             SuperStar.waitDone = true;
    //             SuperStar.state = STRAIGHT1;
    //             pami.setState(moving);
    //             Serial.println("End wait");
    //         }
    //         break;

    //     case STRAIGHT1:
            
    //         Serial.println("Start STRAIGHT1");
    //         pami.middlecontrol(1.3, 0.0, 0.0, false);  // Avancer vers x = 1.0
    //         SuperStar.firstPathDone = true;
    //         SuperStar.state = TURN;
    //         Serial.println("End STRAIGHT1");
            
    //         break;

    //     case TURN:
    //         pami.target_reached = false;
    //         Serial.println("Start TURN");
    //         // Exemple : tourner de 90° à droite
    //         // pami.Turn(90.0);  // Tourner de 90° à droite
    //         pami.middlecontrol(1.3, 1.0, 90.0, false);  // Tourner de 90° à droite
            
    //         SuperStar.turnDone = true;
    //         SuperStar.state = STRAIGHT2;
    //         Serial.println("End TURN");
            
    //         break;

    //     case STRAIGHT2:
    //         pami.middlecontrol(0.0, 2.0, 90.0, false);  // Avancer vers x = 2.0
    //         SuperStar.secondPathDone = true;
    //         SuperStar.state = SWITCH;
    //         Serial.println("End STRAIGHT2");
    //         break;
            


    //     case SWITCH:
    //         pami.turnTail();  // Agite un microswitch
    //         SuperStar.switchActivated = true;
    //         Serial.println("Microswitch activé");
    //         while (true){ 
    //         }
    //         break;
    // }

    // delay(100);

}