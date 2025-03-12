#include "push_cans_function.h"

void setup() {
    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(SWITCH_RIGHT, INPUT);
    pinMode(SWITCH_LEFT, INPUT);
    digitalWrite(ENABLE_PIN, LOW); // Active le moteur
    
    calibration();
}

void loop() {
    // calibration();
    void routine_separation_stack ();
    
}
