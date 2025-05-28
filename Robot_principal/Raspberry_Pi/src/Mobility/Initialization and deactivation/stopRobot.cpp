#include "../../../include/Robot.h"

/**
 * @brief Stop the robot
 * 
 * This function stops the motors.
 */
void Robot::stop() {
    leftMotor.stop();
    rightMotor.stop();
}