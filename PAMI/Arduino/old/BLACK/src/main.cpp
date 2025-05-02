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
double startGame;

int role;
Robot SuperStar;

int middle_state = 'a';
unsigned long state_timer = 0;

enum Team { YELLOW, BLUE };
int team;



void setup() {
    Serial.begin(9600);
    // Starting corde on the microswitch
    while (analogRead(A5) > 1.5) {
        Serial.println("Waiting for the microswitch to be pressed...");
        delay(100); // Attendre un peu avant de vérifier à nouveau
    }

    while (analogRead(A5) < 1.5) {
        Serial.println("Waiting for the microswitch to be released...");
        delay(100); // Attendre un peu avant de vérifier à nouveau
    }
    Serial.println("Microswitch released. Starting the robot.");
    
    //Initialisation du PAMI
    pami = PAMI();

    Serial.println("Début du test de vitesse");
    startGame = millis();

    delay(86000);  // Attendre que le moniteur série soit prêt

    team = YELLOW;
    // team = BLUE;

    }


void team_yellow() {
    pami.update_position();

    switch (middle_state) {
        case 'a':
            if (!pami.target_reached) {
                pami.middlecontrol(0.7, -0.57, 0.0, false, 0.08, startGame);
            } else {
                pami.target_reached = false;
                middle_state = 'b'; // Changer d'état
                state_timer = millis();  // Démarre un timer
                Serial.println("Cible atteinte, changement d'état.");
            }
            break;

        case 'b':
            if (!pami.target_reached) {
                pami.middlecontrol(2.05, -0.47, 0.0, false, 0.1, startGame);
            } else {
                pami.target_reached = false;
                middle_state = 'c'; // Changer d'état
                state_timer = millis();  // Démarre un timer
                Serial.println("Cible atteinte, changement d'état.");
            }
            break;

        case 'c':
            delay(100); // Attendre 1 seconde avant de tourner
            pami.Rotate(60, startGame); // Tourner à droite de 90 degrés
            Serial.println("Rotation de 90 degrés effectuée.");
            middle_state = 'd'; // Changer d'état
            break;

        case 'd':
            pami = PAMI(); // Réinitialiser PAMI
            middle_state = 'e'; // Changer d'état
            break;

        case 'e':
            if (!pami.target_reached) {
                pami.middlecontrol(0.16, 0.05, 0.0, false, 0.04, startGame);
                leftMotor.set_motor(0); // Avancer lentement
                rightMotor.set_motor(0); // Avancer lentement


            } else {
                pami.target_reached = false;
                middle_state = 'f'; // Changer d'état
                state_timer = millis();  // Démarre un timer
                Serial.println("Cible atteinte, changement d'état.");
            }
            break;

        case 'f':
            pami.turnTail();  // Agite la queue
            leftMotor.set_motor(0); // Arrêter le moteur gauche
            rightMotor.set_motor(0); // Arrêter le moteur droit
            Serial.println("Arrêt du robot.");
            break;
    }
}

void team_blue() {
    pami.update_position();

    switch (middle_state) {
        case 'a':
            if (!pami.target_reached) {
                pami.middlecontrol(1, 0.5, 0.0, false, 0.1, startGame);
                // leftMotor.set_motor(0); // Avancer lentement
                // rightMotor.set_motor(0); // Avancer lentement
            } else {
                pami.target_reached = false;
                middle_state = 'b'; // Changer d'état
                state_timer = millis();  // Démarre un timer
                Serial.println("Cible atteinte, changement d'état.");
            }
            break;

        case 'b':
            if (!pami.target_reached) {
                pami.middlecontrol(2.03, -0.8, 0.0, false, 0.12, startGame);
                leftMotor.set_motor(0); // Avancer lentement
                rightMotor.set_motor(0); // Avancer lentement
            } else {
                pami.target_reached = false;
                middle_state = 'c'; // Changer d'état
                state_timer = millis();  // Démarre un timer
                Serial.println("Cible atteinte, changement d'état.");
            }
            break;

        case 'c':
            delay(100); // Attendre 1 seconde avant de tourner
            pami.Rotate(-90, startGame); // Tourner à droite de 90 degrés
            Serial.println("Rotation de 90 degrés effectuée.");
            middle_state = 'd'; // Changer d'état
            break;

        case 'd':
            pami = PAMI(); // Réinitialiser PAMI
            middle_state = 'e'; // Changer d'état
            break;

        case 'e':
            if (!pami.target_reached) {
                pami.middlecontrol(0.21, -0.2, 0.0, false, 0.17, startGame);
                leftMotor.set_motor(0); // Avancer lentement
                rightMotor.set_motor(0); // Avancer lentement


            } else {
                pami.target_reached = false;
                middle_state = 'f'; // Changer d'état
                state_timer = millis();  // Démarre un timer
                Serial.println("Cible atteinte, changement d'état.");
            }
            break;

        case 'f':
            pami.turnTail();  // Agite la queue
            leftMotor.set_motor(0); // Arrêter le moteur gauche
            rightMotor.set_motor(0); // Arrêter le moteur droit
            Serial.println("Arrêt du robot.");
            break;
        }
}





void loop(){
    switch (team) {
        case YELLOW:
            team_yellow();
            break;
        case BLUE:
            team_blue();
            break;
        default:
            Serial.println("Équipe non reconnue.");
            break;
    }
    

   
}

