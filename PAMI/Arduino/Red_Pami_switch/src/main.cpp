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
#define BUZZ 13


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

Robot SuperStar = {STRAIGHT1, 0, false, false, false, false, false};

int role;


void setup() {
    Serial.begin(9600);

    while (analogRead(A5) < 1.5) {
        Serial.println("Waiting for the microswitch to be pressed...");
        delay(100); // Attendre un peu avant de vérifier à nouveau
    }

    int val = analogRead(A1);  // read the input pin
    Serial.println(val); 

    if (analogRead(A1) > 900) {  // team bleu 
        Serial.println("team jaune");
        digitalWrite(BUZZ, HIGH); // Simuler la pression du bouton
        delay(100); // Attendre 1 seconde
        digitalWrite(BUZZ, LOW); // Relâcher le bouton
    }
    else if (analogRead(A1) < 900) { // team jaune
        Serial.println("team bleu");
        digitalWrite(BUZZ, HIGH); // Simuler la pression du bouton
        delay(100); // Attendre 1 seconde
        digitalWrite(BUZZ, LOW); // Relâcher le bouton
        delay(500); // Attendre 1 seconde
        digitalWrite(BUZZ, HIGH); // Simuler la pression du bouton
        delay(100); // Attendre 1 seconde
        digitalWrite(BUZZ, LOW); // Relâcher le bouton
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
    delay(85000);  // Attendre que le moniteur série soit prêt

    
    // pinMode(14, OUTPUT);

    role = 1; // 0 pour le robot, 1 pour la superstar
    val = analogRead(A1);  // read the input pin
    Serial.println(val); 
}


void loop(){   

    pami.update_position();
    // pami.lowlevelcontrol(0.38, 0.38); // Stop the motors
    if(!pami.target_reached){
        if (analogRead(A1) >= 900) { // A changer 
            Serial.println("team jaune");
            pami.middlecontrol(1.1, -0.35, 0.0, false, startGame);  // Avancer vers x = 1.0
        }
        else {
            Serial.println("team bleu");
            pami.middlecontrol(1.05, 0.23, 0.0, false, startGame);  // Avancer vers x = 1.0
        }
    }
    else{
        leftMotor.stop_motor();
        rightMotor.stop_motor();
        pami.turnTail(); // Tourner la queue
    }
    // int sonar_distance = pam i.getSonarDistance();
    // Serial.print("Distance: ");
    // Serial.print(sonar_distance);
    // Serial.println(" cm");


}