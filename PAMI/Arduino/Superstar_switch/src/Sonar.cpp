#include "Sonar.h"


Sonar::Sonar(int pin_trig, int pin_echo):PIN_TRIG(pin_trig),PIN_ECHO(pin_echo){
    pinMode(PIN_TRIG, OUTPUT); // Sets the trigPin as an Output
    pinMode(PIN_ECHO, INPUT); // Sets the echoPin as an Input
}

int Sonar::Sonar_Get_Distance(){
  long duration;
  int sonar_distance ;
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  duration = pulseIn(PIN_ECHO, HIGH, 30000);
  sonar_distance = (duration*.0343)/2;  
  return sonar_distance ;
}