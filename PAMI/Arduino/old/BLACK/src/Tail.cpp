#include "Tail.h"


Tail* Tail::tail = nullptr;

// Fonction de configuration des broches
Tail::Tail(int pin_tail): PIN_TAIL(pin_tail){
  // pinMode(PIN_TAIL, OUTPUT);
  // Servo_Tail.attach(PIN_TAIL);
}

// Fonction pour tourner la queue
void Tail::Turn_tail(){
  pinMode(PIN_TAIL, OUTPUT);
  Servo_Tail.attach(PIN_TAIL); // Attacher le servo à la broche
  Servo_Tail.write(20); // Position neutre
  delay(1000); // Attendre un peu avant de changer la position
  // Servo_Tail.write(40); // Position de mouvement
  // delay(500); // Attendre un peu avant de changer la position
  Servo_Tail.write(60); // Position de mouvement
  delay(1000); // Attendre un peu avant de changer la position
  Servo_Tail.detach(); // Détacher le servo après utilisation
}