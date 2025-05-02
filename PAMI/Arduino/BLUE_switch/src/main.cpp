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

// Caractère Team en fonction de la couleur de l'équipe
#define TEAM_COLOR 'Y' // 'Y' pour orange, 'B' pour bleu
// #define TEAM_COLOR 'B'


// // Initialisation des encodeurs
Encoder leftEncoder(2, A4, true);
Encoder rightEncoder(3, A0, false);

// // Initialisation des moteurs
Motor leftMotor(ENA, DIR1, DIR2, &leftEncoder, true);
Motor rightMotor(ENB, DIR3, DIR4, &rightEncoder, false);

//Initialisation de PAMI
PAMI pami;
bool isStopped = false ; 
double startGame;



int role;
Robot SuperStar;

void setup() {
    Serial.begin(9600);

    while (analogRead(A5) < 1.5) {
        Serial.println("Waiting for the microswitch to be pressed...");
        delay(100); // Attendre un peu avant de vérifier à nouveau
    }

    while (analogRead(A5) > 1.5) {
        Serial.println("Waiting for the microswitch to be released...");
        delay(100); // Attendre un peu avant de vérifier à nouveau
    }
    Serial.println("Microswitch released. Starting the robot.");
    //Initialisation du PAMI
    pami = PAMI();

    Serial.println("Début du test de vitesse");

    startGame = millis();
    delay(86000);  // Attendre que le moniteur série soit prêt

    role = 0; // 0 pour le robot, 1 pour la superstar
    SuperStar = {STRAIGHT1, 0, false, false, false, false, false, false}; // Initialisation de l'état du robot

}


void loop(){

    // // Print time_ms,left_ticks,left_speed,right_ticks,right_speed
    // pami.update_position();
    // leftMotor.set_motor(9);
    // rightMotor.set_motor(9);
    // pami.lowlevelcontrol(0.3, 0.3); // Arrêter le robot
    if (pami.target_reached == false){
        if (digitalRead(A5) == LOW) { // a changer 
            pami.middlecontrol(0.9, -0.43, 0.0, false, startGame);  // Avancer vers x = 1.0
        } else {
            pami.middlecontrol(0.95, 0.45, 0.0, false, startGame);  // Avancer vers x = 1.0
        }
    } else {
        pami.turnTail();  // Agiter la queue
        leftMotor.set_motor(0);
        rightMotor.set_motor(0);
    }



    }

