#include "Tail.h"



Tail* Tail::tail = nullptr;

// Fonction de configuration des broches
Tail::Tail(int pin_tail): PIN_TAIL(pin_tail){
  pinMode(PIN_TAIL, OUTPUT);
  // Servo_Tail.attach(PIN_TAIL);
}

// Fonction pour tourner la queue
void Tail::Turn_tail(){
  pinMode(PIN_TAIL, OUTPUT);
  Servo_Tail.attach(PIN_TAIL); // Attacher le servo à la broche
  // reduce the speed of the servo
  Servo_Tail.write(50); // Position neutre
  delay(500); // Attendre un peu avant de changer la position
  // Servo_Tail.write(40); // Position de mouvement
  // delay(500); // Attendre un peu avant de changer la position
  Servo_Tail.write(140); // Position de mouvement
  delay(500); // Attendre un peu avant de changer la position

  Servo_Tail.detach(); // Détacher le servo après utilisation

}