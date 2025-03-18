#include "../../include/Robot.h"

/**
 * @brief Get the angle of the robot
 * 
 * Computes it based on the difference in distance between the left and right motors.
 * 
 * @return double Angle in degrees
 */
double Robot::getAngle() const {
    double theta = 360 * (rightMotor.getDistance() - leftMotor.getDistance()) / (2 * M_PI * 23.65);
    return theta;
}
