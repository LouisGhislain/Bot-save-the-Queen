#include "../../include/Robot.h"

/**
 * @brief Stop the robot
 * 
 * This function stops the motors.
 */
void Robot::stop() {
    leftMotor.brake();
    rightMotor.brake();
    usleep(10000000);
    leftMotor.stop();
    rightMotor.stop();
}