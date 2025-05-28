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

bool Robot::is_right_pressed(GAME *game){
    if(digitalRead(MICROSWITCH_BACK_RIGHT) == HIGH){
        return true ; 
    } else{
        return false ;
    }
}

bool Robot::is_left_pressed(GAME *game){
    if(digitalRead(MICROSWITCH_BACK_LEFT) == HIGH){
        return true ; 
    } else{
        return false ;
    } 
}