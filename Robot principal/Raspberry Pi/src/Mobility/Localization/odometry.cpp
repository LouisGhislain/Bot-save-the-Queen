#include "../../../include/Robot.h"

/*! \brief Odometry
 * Updates the robot position
 * 
 * This function computes the robot's x and y coordinates and its angle based on the wheel speeds over time
 */
void Robot::updateOdometry(void *sqid){

    GAME * squid = (GAME *)sqid;
    Queen * queen = squid->queen;   

    // Get distance travelled by each wheel
    distl = leftMotor.getDistance(); // in m
    distr = rightMotor.getDistance(); // 

    {
        std::lock_guard<std::mutex> lock(queen->position_mutex);
        // Compute the angle of the robot
        queen->angle = ((distr - distl) / (distanceBetweenOdometers)) + starting_angle; // in radians //corrective factor 0.99810435
        // Normalize angle to range [-π, π]
        while (queen->angle > M_PI) {    
            queen->angle -= 2 * M_PI;
        }
        while (queen->angle <= -M_PI) {
            queen->angle += 2 * M_PI;
        }

        // Compute the position of the robot
        double displacement = (distl - last_distl + distr - last_distr) / 2;
        queen->cart_pos->x = queen->cart_pos->x + displacement * cosl(queen->angle);
        queen->cart_pos->y = queen->cart_pos->y + displacement * sinl(queen->angle);
    }
    // Update last distances
    last_distl = distl; 
    last_distr = distr;

    // Print for verification
    // printf("X: %.2f, Y: %.2f, Theta: %.2f\n", xCoord, yCoord, theta);

}

void Robot::initCoords(void *sqid) {
    GAME *squid = (GAME *)sqid;
    Queen * queen = squid->queen;
    // pas besoin de mutex ici car initCoords est appelé une seule fois au début avant que le thread ne soit créé

    switch (starting_pos)
    {
        case 0: // Blue bottom
            queen->cart_pos->x = 1.8;        // in m (origin at the bottom left, angle 0 = x-axis)
            queen->cart_pos->y = 0.19184;     // in m
            starting_angle     = M_PI/2;       // in radians
            break;

        case 1: // Blue side
            queen->cart_pos->x = 0.3673;
            queen->cart_pos->y = 0.8365;
            starting_angle     = 0.0;
            break;

        case 2: // Yellow bottom
            queen->cart_pos->x = 1.2;          
            queen->cart_pos->y = 0.19184;
            starting_angle     = M_PI/2;
            break;

        case 3: // Yellow side
            queen->cart_pos->x = 2.6327;
            queen->cart_pos->y = 0.8365;
            starting_angle     = M_PI;
            break;
        default:
            fprintf(stderr, "No starting position specified, do it !\n");
            break;
    }
    GLOBAL_x_coord_target = queen->cart_pos->x;
    GLOBAL_y_coord_target = queen->cart_pos->y;

}
