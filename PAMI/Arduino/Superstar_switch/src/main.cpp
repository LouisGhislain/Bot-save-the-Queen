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
// #define TEAM_COLOR 'Y' // 'Y' pour jaune, 'B' pour bleu
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

char TEAM_COLOR; // 'Y' pour jaune, 'B' pour bleu


int role;
Robot SuperStar;

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

        pami.turnTail_start(25);  // Agiter la queue

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

        pami.turnTail_start(50);  // Agiter la queue

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


    role = 0; // 0 pour le robot, 1 pour la superstar
    SuperStar = {STRAIGHT1, 0, false, false, false, false, false}; // Initialisation de l'état du robot

}

void Superstar_ORANGE(){

    switch(SuperStar.state) {
    
        case STRAIGHT1:
            
            Serial.println("Start STRAIGHT1");
            pami.middlecontrol(1.15, -0.1, 0.0, false, startGame);  // Avancer vers x = 1.0
            SuperStar.firstPathDone = true;
            SuperStar.state = TURN;
            Serial.println("End STRAIGHT1");
            
            break;

        case TURN:
            pami.target_reached = false;
            Serial.println("Start TURN");
            // Exemple : tourner de 90° à droite
            delay(200);
            pami.Rotate(-55, startGame);  // Tourner de 90° à droite
            // Serial.println(pami.getAngle());
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
            while (millis() - t_start < 250) {
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
            SuperStar.switchActivated = true;
            break;


    }
}


void Superstar_BLUE(){

    switch(SuperStar.state) {
    
        case STRAIGHT1:
            
            Serial.println("Start STRAIGHT1");
            pami.middlecontrol(1.15, 0, 0.0, false, startGame);  // Avancer vers x = 1.0
            SuperStar.firstPathDone = true;
            SuperStar.state = TURN;
            Serial.println("End STRAIGHT1");
            
            break;

        case TURN:
            pami.target_reached = false;
            Serial.println("Start TURN");
            // Exemple : tourner de 90° à droite
            pami.Rotate(45, startGame);  // Tourner de 90° à droite
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
            while (millis() - t_start < 250) {
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
            SuperStar.switchActivated = true;
            break;


    }
}


void loop(){

    if (TEAM_COLOR == 'B') { // changer le pin 12 par le pin du switch utilisé 
        Superstar_BLUE();
    } else if (TEAM_COLOR == 'Y') {
        Superstar_ORANGE();
    }
    else {
        Serial.println("Invalid TEAM_COLOR. Please set it to 'Y' or 'B'.");
    }

    // Print time_ms,left_ticks,left_speed,right_ticks,right_speed
    // pami.update_position();
    // // leftMotor.set_motor(9);
    // // rightMotor.set_motor(9);
    // // pami.lowlevelcontrol(0.3, 0.3); // Arrêter le robot
    // if (pami.target_reached == false){
    //     pami.middlecontrol(1, -0.5, 0.0, false);  // Avancer vers x = 1.0
    // } else {
    //     leftMotor.set_motor(0);
    //     rightMotor.set_motor(0);
    //     pami.pami_brake(); // Freiner le robot
    // }

    }

