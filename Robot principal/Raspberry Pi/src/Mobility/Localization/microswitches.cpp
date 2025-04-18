#include "../../../include/Robot.h"

bool Robot::checkIfAgainstWall(GAME *game) {
    if (digitalRead(MICROSWITCH_BACK_LEFT) == HIGH && digitalRead(MICROSWITCH_BACK_RIGHT) == HIGH) {
        std::cout << "Both microswitches are HIGH" << std::endl;
        return true;
    } else {
        // Print the state of each switch
        //std::cout << "Microswitch Back Left: " << (digitalRead(MICROSWITCH_BACK_LEFT) == HIGH ? "HIGH" : "LOW") << std::endl;
        //std::cout << "Microswitch Back Right: " << (digitalRead(MICROSWITCH_BACK_RIGHT) == HIGH ? "HIGH" : "LOW") << std::endl;
        return false;
    }
}