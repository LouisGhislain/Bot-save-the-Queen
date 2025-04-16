#include "Tail.h"


Tail* Tail::tail = nullptr;

// Fonction de configuration des broches
Tail::Tail(int pin_tail): PIN_TAIL(pin_tail){
  pinMode(PIN_TAIL, OUTPUT);
  // Servo_Tail.attach(PIN_TAIL);
}

// Méthode pour activer le servo-moteur
void Tail::ActivateServo() {
  Servo_Tail.attach(PIN_TAIL); // Attacher le servo uniquement quand nécessaire
}

// Fonction pour tourner la queue
void Tail::Turn_tail(){
  // int tail_pos = 0;   
  // for (tail_pos = 55; tail_pos <= 140; tail_pos += 5) { // goes from 0 degrees to 180 degrees
  //     Servo_Tail.write(tail_pos);              // tell servo to go to position in variable 'pos'
  //     delay(50);                       // waits 15ms for the servo to reach the position
  //   }
  //   for (tail_pos = 140; tail_pos >= 55; tail_pos -= 5) { // goes from 180 degrees to 0 degrees
  //     Servo_Tail.write(tail_pos);              // tell servo to go to position in variable 'pos'
  //     delay(50);                       // waits 15ms for the servo to reach the position
  //   }
  Servo_Tail.write(55); // Position neutre
  delay(1000); // Attendre un peu avant de changer la position
  Servo_Tail.write(97); // Position de mouvement
  delay(500); // Attendre un peu avant de changer la position
  Servo_Tail.write(140); // Position de mouvement
  delay(1000); // Attendre un peu avant de changer la position
  Servo_Tail.write(97); // Position de mouvement
  delay(500); // Attendre un peu avant de changer la position
  Servo_Tail.write(55); // Position neutre
  delay(1000); // Attendre un peu avant de changer la position
}