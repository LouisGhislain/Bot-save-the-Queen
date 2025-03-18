#include "../../include/Robot.h"

#define BUZZER_PIN 22  // GPIO22 corresponds to WiringPi pin 3
void Robot::buzzBuzzer(){
    // Set the buzzer pin as output
    pinMode(BUZZER_PIN, OUTPUT);

    std::cout << "Buzzer test started. Press Ctrl+C to exit." << std::endl;

    // Turn the buzzer on
    digitalWrite(BUZZER_PIN, HIGH);
    std::cout << "Buzzer ON" << std::endl;
    delay(1000);  // Wait for 1 second

    // Turn the buzzer off
    digitalWrite(BUZZER_PIN, LOW);
    std::cout << "Buzzer OFF" << std::endl;
}
