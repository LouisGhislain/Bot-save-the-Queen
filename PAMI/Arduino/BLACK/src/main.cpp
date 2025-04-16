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



int role;
Robot SuperStar;

void setup() {
    Serial.begin(9600);

    //Initialisation du PAMI
    pami = PAMI();

    Serial.println("Début du test de vitesse");

    delay(5000);  // Attendre que le moniteur série soit prêt

    startTime = millis();


    role = 0; // 0 pour le robot, 1 pour la superstar
    SuperStar = {STRAIGHT1, 0, false, false, false, false, false, false}; // Initialisation de l'état du robot

}


void Superstar(){

    switch(SuperStar.state) {
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
            pami.Rotate(-60);  // Tourner de 90° à droite
            Serial.println(pami.getAngle());
            // pami.middlecontrol_switch(1.11, -1, 90.0, false);  // Tourner de 90° à droite
            
            SuperStar.turnDone = true;
            SuperStar.state = STRAIGHT2;
            Serial.println("End TURN");
            //Fait un signal buzzer sur la pin 13
            digitalWrite(13, HIGH); // Activer le buzzer

            
            break;

        case STRAIGHT2: {
            while(digitalRead(12) == HIGH){
                Serial.println(pami.getAngle());
                pami.lowlevelcontrol(0.1, 0.1); // Avancer tout droit
            }
        
            double t_start = millis();
            while (millis() - t_start < 150) {
                pami.lowlevelcontrol(0.1, 0.1); // Avancer tout droit
            }
        
            pami.pami_brake();
            SuperStar.secondPathDone = true;
            SuperStar.state = QUEUE;
            Serial.println("End STRAIGHT2");
            break;
        }

        case QUEUE:
            Serial.println("Start QUEUE");
        
            pami.turnTail();  // Agite la queue
            // Servo Servo_Tail;
            // Servo_Tail.attach(9);
            // Servo_Tail.write(60); // Position neutre
            // delay(1000); // Attendre un peu avant de changer la position
            // Servo_Tail.write(110); // Position de mouvement
            // delay(1000); // Attendre un peu avant de changer la position
            // Servo_Tail.write(60); // Position neutre
            // delay(1000); // Attendre un peu avant de changer la position
            // Servo_Tail.detach(); // Détacher le servo après utilisation
            SuperStar.switchActivated = true;
            break;


    }
}


void loop(){

    // switch(role){
    //     case 0:
    //         // Agiter la queue
    //         pami.turnTail();  // Agite la queue
    //         Serial.println("Agitation de la queue terminée.");
    //         break;

    //     case 1:
    //         Superstar();
    //         break;

    // }

    // Print time_ms,left_ticks,left_speed,right_ticks,right_speed
    pami.update_position();
    // leftMotor.set_motor(9);
    // rightMotor.set_motor(9);
    // pami.lowlevelcontrol(0.3, 0.3); // Arrêter le robot
    if (pami.target_reached == false){
        pami.middlecontrol(2, 0, 0.0, false);  // Avancer vers x = 1.0
    } else {
        leftMotor.set_motor(0);
        rightMotor.set_motor(0);
        // pami.pami_brake(); // Freiner le robot
    }
    }

