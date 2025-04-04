#include "../../../include/Robot.h"

/*! \brief Odometry
 * Updates the robot position
 * 
 * This function computes the robot's x and y coordinates and its angle based on the wheel speeds over time
 */
void Robot::updateOdometry(void *sqid){

    GAME * squid = (GAME *)sqid;
    Robot& robot = *squid->robot;   

    // Get distance travelled by each wheel
    distl = leftMotor.getDistance(); // in m
    distr = rightMotor.getDistance(); // Corrective factor for right wheel = 0.997126437

    // Compute the angle of the robot
    robot->angle = 0.999683332 * ((distr - distl) / (distanceBetweenOdometers)) + starting_angle; // in radians //corrective factor 0.99810435
    // Normalize angle to range [-π, π]
    while (robot->angle > M_PI) {    
        robot->angle -= 2 * M_PI;
    }
    while (robot->angle <= -M_PI) {
        robot->angle += 2 * M_PI;
    }

    // Compute the position of the robot
    double displacement = (distl - last_distl + distr - last_distr) / 2;
    robot->cart_pos->x = robot->cart_pos->x + displacement * cosl(robot->angle);
    robot->cart_pos->y = robot->cart_pos->y + displacement * sinl(robot->angle);

    // Update last distances
    last_distl = distl; 
    last_distr = distr;

    // Print for verification
    // printf("X: %.2f, Y: %.2f, Theta: %.2f\n", xCoord, yCoord, theta);

}

void Robot::initCoords(void *sqid) {
    GAME *squid = (GAME *)sqid;
    Robot& robot = *squid->robot; 

    switch (starting_pos)
    {
    case 0: // Blue bottom
        robot->cart_pos->x = 0.3;  // in m (origin at the bottom left, angle 0 = x-axis)
        robot->cart_pos->y = 0.9;  // in m
        robot->angle     = 0.0;   // in radians
        break;
    case 1: // Blue side
        robot->cart_pos->x = 2.85;
        robot->cart_pos->y = 0.7;
        robot->angle     = 0.0;
        break;
    case 2: // Yellow bottom
        robot->cart_pos->x = 2.7;
        robot->cart_pos->y = 0.9;
        robot->angle     = M_PI;
        break;
    case 3: // Yellow side
        robot->cart_pos->x = 0.15;
        robot->cart_pos->y = 0.7;
        robot->angle     = M_PI;
        break;
    default: // Default starting position
        robot->cart_pos->x = 0.3;
        robot->cart_pos->y = 0.9;
        robot->angle     = 0.0;
        break;
    }
}
