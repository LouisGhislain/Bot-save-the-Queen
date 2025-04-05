#include <Arduino.h>
#include <Servo.h>
#include "Tail.h"

#define Tail 12

Servo Servo_Tail;

int tail_pos = 0;          // Variable pour stocker la position de la queue

// Fonction de configuration des broches
void Pin_Tail_Initialization(){
    pinMode(Tail, OUTPUT);
    Servo_Tail.attach(Tail);
}

// Fonction pour tourner la queue
void Turn_tail(){
    for (tail_pos = 55; tail_pos <= 140; tail_pos += 5) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        Servo_Tail.write(tail_pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      for (tail_pos = 140; tail_pos >= 55; tail_pos -= 5) { // goes from 180 degrees to 0 degrees
        Servo_Tail.write(tail_pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }

}