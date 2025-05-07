#include "../../include/Robot.h"

void Robot::buzzBuzzer(){
    // Turn the buzzer on
    digitalWrite(BUZZER_PIN, HIGH);
    std::cout << "Buzzer ON" << std::endl;
    delay(1000);  // Wait for 1 second

    // Turn the buzzer off
    digitalWrite(BUZZER_PIN, LOW);
    std::cout << "Buzzer OFF" << std::endl;
}