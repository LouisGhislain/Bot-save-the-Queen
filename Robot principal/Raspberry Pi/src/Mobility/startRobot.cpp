#include "../../include/Robot.h"

/**
 * @brief Start the robot
 * 
 * This function initializes the motors and resets the integral error terms.
 * 
 */
void Robot::start() {
    initializeSPI();
    resetValues();
    intEPosLeft = intEPosRight = 0.0;
    intESpeedLeft = intESpeedRight = 0.0;
}