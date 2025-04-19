#include <Arduino.h>
#include <Servo.h>
#include "Motor.h"
#include "Encoder.h"
#include "Tail.h"
#include "Sonar.h"
#include "Pami.h"
#include "Struct.h"

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

Robot SuperStar = {STRAIGHT1, 0, false, false, false, false, false};

int role;


void setup() {
    Serial.begin(9600);

    //Initialisation du PAMI
    pami = PAMI();

    Serial.println("Début du test de vitesse");

    delay(5000);  // Attendre que le moniteur série soit prêt

    startTime = millis();
    pinMode(14, OUTPUT);

    role = 1; // 0 pour le robot, 1 pour la superstar

}


void Superstar(){

    switch (SuperStar.state) {
        case WAIT:
            if (SuperStar.startTime == 0) {
                SuperStar.startTime = millis();
                Serial.println("Start wait");
            }
            if (millis() - SuperStar.startTime >= 85000) {
                SuperStar.waitDone = true;
                SuperStar.state = STRAIGHT1;
                pami.setState(moving);
                Serial.println("End wait");
            }
            break;

        case STRAIGHT1:
            
            Serial.println("Start STRAIGHT1");
            pami.middlecontrol(1.15, -0.05, 0.0, false);  // Avancer vers x = 1.0
            SuperStar.firstPathDone = true;
            SuperStar.state = TURN;
            Serial.println("End STRAIGHT1");
            
            break;

        case TURN:
            pami.target_reached = false;
            Serial.println("Start TURN");
            // Exemple : tourner de 90° à droite
            pami.Rotate(-93);  // Tourner de 90° à droite
            // pami.middlecontrol_switch(1.11, -1, 90.0, false);  // Tourner de 90° à droite
            
            SuperStar.turnDone = true;
            SuperStar.state = STRAIGHT2;
            Serial.println("End TURN");
            
            break;

        case STRAIGHT2:
            pami = PAMI();
            Serial.println(digitalRead(12));
            while(digitalRead(12) == HIGH){
                Serial.println(digitalRead(12));
                pami.lowlevelcontrol(0.1, 0.1); // Avancer tout droit
            }
            // Pendant 0.5s faire encore lowlevelcontrol
            double t_start = millis();
            while (millis() - t_start < 150) {
                pami.lowlevelcontrol(0.1, 0.1); // Avancer tout droit
            }
            Serial.println(digitalRead(12));

            Serial.println("End STRAIGHT2");
            pami.pami_brake(); // Freiner le robot
            SuperStar.secondPathDone = true;
            SuperStar.state = SWITCH;
            break;

        case SWITCH:
            pami.turnTail();  // Agite un microswitch
            SuperStar.switchActivated = true;
            Serial.println("Microswitch activé");
            while (true){ 
                // pour qu'il ne bouge plus 
            }
            break;
    }


}


void loop(){

    pami.turnTail();  // Agite un microswitch
    // if (!pami.target_reached){
    //     pami.middlecontrol(1.14, -0.45, 0.0, false);  // Avancer vers x = 1.0
    // }
    // else {
    //     pami.turnTail();  // Agite un microswitch
    //     leftMotor.set_motor(0);
    //     rightMotor.set_motor(0); // Arrêter le moteur droit
    //     pami.pami_brake(); // Freiner le robot
    // }

}