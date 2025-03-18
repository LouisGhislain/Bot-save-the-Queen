#include "../../include/Robot.h"

/**
 * @brief Middle-level controller - Go to a position
 * 
 * This function implements a middle-level controller that creates a trajectory to a target position.
 * 
 * @param x X-coordinate of the target position
 * @param y Y-coordinate of the target position
 * @param goal_angle Angle to the target position
 */
void Robot::middleLevelController(double x, double y, double goal_angle) {
    // Initial distance value
    double rho = 1000;
    
    while (rho > 10) {
        // Calculate distance to target (distance error)
        rho = std::sqrt(std::pow(x - xCoord, 2) + std::pow(y - yCoord, 2)); // Distance in cm
    
        // Calculate angle to target (angle error)
        double alpha = std::atan2(y - yCoord, x - xCoord) * (360 / (2 * M_PI)) - getAngle(); // Angle in degrees
    
        double beta = goal_angle - getAngle() - alpha; // Angle in degrees
    
        // Calculate speed references
        double v = KpRho * rho + 0.2;
        double w = KpAlpha * alpha + KpBeta * beta;
        double ref_speed_left = v - w;
        double ref_speed_right = v + w;
    
        // Control loop
        lowLevelController(ref_speed_left, ref_speed_right);
    
        // Update position
        double displacement = (leftMotor.getDistance() - lastLeftDistance + rightMotor.getDistance() - lastRightDistance) / 2;
    
        xCoord += displacement * std::cos((getAngle() / 360) * 2 * M_PI) * (100 / 70.2);
        yCoord += displacement * std::sin((getAngle() / 360) * 2 * M_PI) * (100 / 70.2);
    
        lastLeftDistance = leftMotor.getDistance();
        lastRightDistance = rightMotor.getDistance();
    
        // Sleep (equivalent to Python's sleep)
        usleep(SAMPLING_TIME * 1e6); // Convert to microseconds
        }
    
    stop();
    }