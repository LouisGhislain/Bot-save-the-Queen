#include "../../../include/Robot.h"

/*! \brief Odometry
 * Updates the robot position
 * 
 * This function computes the robot's x and y coordinates and its angle based on the wheel speeds over time
 */
void Robot::updateOdometry(){

    // Get distance travelled by each wheel
    distl = leftMotor.getDistance(); // in m
    distr = rightMotor.getDistance(); // Corrective factor for right wheel = 0.997126437

    // Compute the angle of the robot
    sv.theta = 0.999683332 * ((distr - distl) / (distanceBetweenOdometers)) + starting_angle; // in radians //corrective factor 0.99810435
    // Normalize angle to range [-π, π]
    while (sv.theta > M_PI) {
        sv.theta -= 2 * M_PI;
    }
    while (sv.theta <= -M_PI) {
        sv.theta += 2 * M_PI;
    }

    // Compute the position of the robot
    double displacement = (distl - last_distl + distr - last_distr) / 2;
    sv.xCoord = sv.xCoord + displacement * cosl(sv.theta);
    sv.yCoord = sv.yCoord + displacement * sinl(sv.theta);

    // Update last distances
    last_distl = distl; 
    last_distr = distr;

    // Print for verification
    // printf("X: %.2f, Y: %.2f, Theta: %.2f\n", xCoord, yCoord, theta);

}

void Robot::initCoords(){
    switch (starting_pos)
    {
    case 0: // Blue bottom
        sv.xCoord = 0.3;             // in m (with 0,0 at the low left corner, angle 0 = x-axis)
        sv.yCoord = 0.9;             // in m
        starting_angle = 0.0;     // in radians
        break;
    case 1: // Blue side
        sv.xCoord = 2.85;
        sv.yCoord = 0.7;
        starting_angle = 0.0;
        break;
    case 2: // Yellow bottom
        sv.xCoord = 2.7;
        sv.yCoord = 0.9;
        starting_angle = M_PI;
        break;
    case 3: // Yellow side
        sv.xCoord = 0.15;
        sv.yCoord = 0.7;
        starting_angle = M_PI;
        break;    
    default: // Default case (if no starting position is set)
        sv.xCoord = 0.3;
        sv.yCoord = 0.9;
        starting_angle = 0.0;
        break;
    }
}
