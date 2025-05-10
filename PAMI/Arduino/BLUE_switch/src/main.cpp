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
// #define TEAM_COLOR 'Y' // 'Y' pour orange, 'B' pour bleu
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

char TEAM_COLOR; // 'Y' pour orange, 'B' pour bleu

void setup() {
    Serial.begin(9600);

    while (analogRead(A5) < 1.5) {
        Serial.println("Waiting for the microswitch to be pressed...");
        delay(100); // Attendre un peu avant de vérifier à nouveau
    }

    int sensorValueSum = 0;
    int numReadings = 10;  // nombre d'échantillons sur 1 seconde
    int threshold = 800;    // seuil pour distinguer HIGH/LOW (sur 10 bits ADC)

    for (int i = 0; i < numReadings; i++) {
        sensorValueSum += analogRead(A1);
        Serial.print("Valeur de la broche A1 : ");
        Serial.println(analogRead(A1)); // Afficher la valeur lue sur le moniteur série
        Serial.print(sensorValueSum);
        delay(100);  // 100 échantillons x 10ms = 1000ms = 1s
    }

    int averageValue = sensorValueSum / numReadings;
    Serial.print("Valeur moyenne de la broche A1 : ");
    Serial.println(averageValue); // Afficher la valeur lue sur le moniteur série

    if (averageValue < threshold) {  // team jaune
        Serial.println("team jaune");
        digitalWrite(BUZZ, HIGH);
        delay(100);
        digitalWrite(BUZZ, LOW);

        TEAM_COLOR = 'Y';
    } else {  // team bleu
        Serial.println("team bleu");
        digitalWrite(BUZZ, HIGH);
        delay(100);
        digitalWrite(BUZZ, LOW);
        delay(500);
        digitalWrite(BUZZ, HIGH);
        delay(100);
        digitalWrite(BUZZ, LOW);

        TEAM_COLOR = 'B';
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
    delay(6000);  // Attendre que le moniteur série soit prêt

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
        if (TEAM_COLOR == 'Y') { // a changer 
            Serial.println("team jaune");
            pami.middlecontrol(0.9, -0.43, 0.0, false, startGame);  // Avancer vers x = 1.0
        } else if (TEAM_COLOR == 'B') { // a changer
            Serial.println("team bleu");
            pami.middlecontrol(0.95, 0.45, 0.0, false, startGame);  // Avancer vers x = 1.0
        }
        else {
            Serial.println("Erreur de couleur d'équipe !");
        }
    } else {
        pami.turnTail();  // Agiter la queue
        leftMotor.set_motor(0);
        rightMotor.set_motor(0);
    }

    // int data = analogRead(A1); // Lire la valeur de la broche A5
    // Serial.print("Valeur de la broche A1 : ");
    // Serial.println(data); // Afficher la valeur lue sur le moniteur série

    }

