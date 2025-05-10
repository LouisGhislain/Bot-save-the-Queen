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

Robot SuperStar = {STRAIGHT1, 0, false, false, false, false, false};

int role;

char TEAM_COLOR; // 'Y' pour orange, 'B' pour bleu


void setup() {
    Serial.begin(9600);

    while (analogRead(A5) < 1.5) {
        Serial.println("Waiting for the microswitch to be pressed...");
        delay(100); // Attendre un peu avant de vérifier à nouveau
    }

    int sensorValueSum = 0;
    int delayTime = 100; // Temps d'attente entre les lectures (en millisecondes)
    int numReadings = 1000/delayTime;  // nombre d'échantillons sur 1 seconde
    int threshold = 900;    // seuil pour distinguer HIGH/LOW (sur 10 bits ADC)

    for (int i = 0; i < numReadings; i++) {
        sensorValueSum += analogRead(A1);
        Serial.print("Valeur de la broche A1 : ");
        Serial.println(analogRead(A1)); // Afficher la valeur lue sur le moniteur série
        Serial.print(sensorValueSum);
        delay(delayTime);  // 100 échantillons x 10ms = 1000ms = 1s
    }

    int averageValue = sensorValueSum / numReadings;
    Serial.print("Valeur moyenne de la broche A1 : ");
    Serial.println(averageValue); // Afficher la valeur lue sur le moniteur série

    if (averageValue < threshold) {  // team jaune
        Serial.println("team jaune");
        digitalWrite(BUZZ, HIGH);
        delay(100);
        digitalWrite(BUZZ, LOW);

        pami.turnTailYellow(90); // Tourner la queue
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

        pami.turnTailYellow(140); // Tourner la queue

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
    delay(5000);  // Attendre que le moniteur série soit prêt

    
    // pinMode(14, OUTPUT);

    role = 1; // 0 pour le robot, 1 pour la superstar
}


void loop(){   

    pami.update_position();
    // pami.lowlevelcontrol(0.38, 0.38); // Stop the motors
    if(!pami.target_reached){
        if (TEAM_COLOR == 'Y') { // A changer 
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