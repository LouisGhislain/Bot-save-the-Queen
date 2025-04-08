#include "../../../include/Robot.h"

/**
 * @brief Stop the robot
 * 
 * This function stops the motors.
 */
void Robot::stop() {
    leftMotor.brake();
    rightMotor.brake();
    usleep(1000000); // break during 1 second
    leftMotor.stop();
    rightMotor.stop();
}