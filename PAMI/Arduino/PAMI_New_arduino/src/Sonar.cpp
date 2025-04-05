#include <Arduino.h>
#include "Sonar.h"


long duration;
int sonar_distance ;

void Pin_Sonar_Initialization(){
    pinMode(Sonar_trig, OUTPUT); // Sets the trigPin as an Output
    pinMode(Sonar_echo, INPUT); // Sets the echoPin as an Input
    //state_asking = false ; 
}

int Sonar_Get_Distance(){
  //state_asking = true ; 
  digitalWrite(Sonar_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Sonar_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Sonar_trig, LOW);
  duration = pulseIn(Sonar_echo, HIGH, 30000);
  sonar_distance = (duration*.0343)/2;  
  //state_asking = false ; 
  return sonar_distance ; 
  /*
  // Clears the trigPin
  state_asking = true ; 
  digitalWrite(Sonar_trig, LOW);
  delayMicroseconds(5);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(Sonar_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Sonar_trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  while (pulseIn(Sonar_echo, HIGH)==0){
  }
  duration = pulseIn(Sonar_echo, HIGH);
  // Calculating the distance
  sonar_distance = duration * 3.40 / 2;
  state_asking = false ; 
  return sonar_distance ;
  */ 
}