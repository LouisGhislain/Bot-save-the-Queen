#include "../../include/Robot.h"

/*! \brief Odometry
 * Updates the robot position
 * 
 * This function computes the robot's x and y coordinates and its angle based on the wheel speeds over time
 */
void updateOdometry(){
    // Update last distances
    last_distl = distl;
    last_distr = distr;

    // Get distance travelled by each wheel
    distl = distl + (leftMotor.getSpeed() * SAMPLING_TIME * WHEEL_RADIUS); // in m
    distr = distr + (rightMotor.getSpeed() * SAMPLING_TIME * WHEEL_RADIUS);

    // Compute the angle of the robot
    theta = ((distr - distl) / (distanceBetweenWheels)) + starting_angle; // in radians
    // Normalize angle to range [-π, π]
    while (theta > M_PI) {
        theta -= 2 * M_PI;
    }
    while (theta <= -M_PI) {
        theta += 2 * M_PI;
    }

    // Compute the position of the robot
    double displacement = (distl - last_distl + distr - last_distr) / 2;
    xCoord = xCoord + displacement * cos(theta);
    yCoord = yCoord + displacement * sin(theta);

    // Print for verification
    // printf("-Odometry- x:%f [m] ; y:%f [m] ; angle:%f [deg]\n", cvs->xCoord, cvs->yCoord, (cvs->theta)/M_PI*180);
    
}

void initcoords(){
    switch (starting_pos)
    {
    case 0: // Blue bottom
        xCoord = 0.3;             // in m (with 0,0 at the low left corner, angle 0 = x-axis)
        yCoord = 0.9;             // in m
        starting_angle = 0.0;     // in radians
        break;
    case 1: // Blue side
        cvs->xCoord = 2.85;
        cvs->yCoord = 0.7;
        cvs->starting_angle = 0.0;
        break;
    case 2: // Yellow bottom
        cvs->xCoord = 2.7;
        cvs->yCoord = 0.9;
        cvs->starting_angle = M_PI;
        break;
    case 3: // Yellow side
        cvs->xCoord = 0.15;
        cvs->yCoord = 0.7;
        cvs->starting_angle = M_PI;
        break;    
    default: // Default case (if no starting position is set)
        cvs->xCoord = 0.3;
        cvs->yCoord = 0.9;
        cvs->starting_angle = 0.0;
        break;
    }
}
