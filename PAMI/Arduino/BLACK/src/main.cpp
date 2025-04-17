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

int middle_state = 0;
unsigned long state_timer = 0;


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

    delay(5000);  // Attendre que le moniteur série soit prêt

    startTime = millis();


    role = 0; // 0 pour le robot, 1 pour la superstar

    }

void loop(){

    pami.update_position();

    switch (middle_state) {
        case 0:
            if (!pami.target_reached) {
                pami.middlecontrol(1, -0.5, 0.0, false, 0.08);
            } else {
                pami.target_reached = false;
                middle_state++;
                state_timer = millis();  // Démarre un timer
                Serial.println("Cible atteinte, changement d'état.");
            }
            break;

        case 1:
            
            if (!pami.target_reached) {
                pami.middlecontrol(1.3, -0.5, 0.0, false, 0.15);
            } else {
                pami.target_reached = false;
                middle_state++;
                state_timer = millis();
                Serial.println("Cible atteinte, changement d'état.");
            }
            
            break;

        case 2:
            
            if (!pami.target_reached) {
                pami.middlecontrol(1.7, -0.5, 0.0, false, 0.2);
            } else {
                pami.target_reached = false;
                middle_state++;
                state_timer = millis();
                Serial.println("Cible atteinte, changement d'état.");
            }
            
            break;

        case 3:
        
            if (!pami.target_reached) {
                pami.middlecontrol(1.9, 0.2, 0.0, false, 0.25);
            } else {
                middle_state++;
                Serial.println("Tous les déplacements terminés.");
            }
            
            break;

        case 4:
            // Tous les déplacements sont faits
            pami.pami_brake(); // Freiner le robot
            leftMotor.set_motor(0);
            rightMotor.set_motor(0);
            Serial.println("Arrêt du robot.");
            pami.turnTail();  // Agite la queue
            break;
    }
}

