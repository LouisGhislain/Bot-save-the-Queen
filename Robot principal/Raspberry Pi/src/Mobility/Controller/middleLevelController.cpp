#include "../../../include/Robot.h"

/**
 * @brief Middle-level controller - Go to a position
 * 
 * This function implements a middle-level controller that creates a trajectory to a target position.
 * 
 * @param x X-coordinate of the target position
 * @param y Y-coordinate of the target position
 * @param goal_angle Angle to the target position (if not important, put anything)
 * @param params Movement parameters
 * @param sqid Pointer to the game structure
 */
void Robot::middleLevelController(double x_coord_target, double y_coord_target, double goal_angle, const MovementParams& params, void *sqid) {
    GAME * squid = (GAME *)sqid;
    Queen * queen = squid->queen;
    
    // delta to the target
    delta_x_target = x_coord_target - queen->cart_pos->x; // in m
    delta_y_target = y_coord_target - queen->cart_pos->y; // in m

    travelled_distance += abs(((distl + distr)-(last_distl + last_distr))/2);
    rho = sqrt(pow(delta_x_target, 2) + pow(delta_y_target, 2)); 

    // Stop the robot when it reaches the target
    if (rho < params.stop_robot_distance){
        if(params.activated_target_angle == 1){
            lowLevelController(0, 0);
            travelled_distance = 0;           // Move feature to high level controller when ready
            std::cout << "Point reached (maybe not orientation) " << std::endl;

            //continue; // if the target is reached but the angle is not, continue to rotate 
        }
        else{
            lowLevelController(0, 0);
            travelled_distance = 0;           // Move feature to high level controller when ready
            std::cout << "Point reached" << std::endl;
            return;
        }
    }

    double alpha =  atan2(delta_y_target, delta_x_target) - queen->angle;
    // Normalize alpha to range [-π, π]
    while (alpha > M_PI) {
        alpha -= 2 * M_PI;
    }
    while (alpha <= -M_PI) {
        alpha += 2 * M_PI;
    }
    double w_ref = KpAlpha * alpha;

    if(params.activated_target_angle == 1){
        // Convert target angle from degrees to radians and normalize it to range [-π, π]
        double rad_target_angle = goal_angle * M_PI/180; // goal angle * 2 * M_PI/360;
        double beta = rad_target_angle - queen->angle - alpha; //Uncomment this line to use target angle
        while (beta > M_PI) {
            beta -= 2 * M_PI;
        }
        while (beta <= -M_PI) {
            beta += 2 * M_PI;
        }
        w_ref += KpBeta * beta;
    }
    w_ref = std::clamp(w_ref, -params.wmax, params.wmax); // Limit the angular speed


    double rot_part = abs(distanceBetweenWheels * w_ref / 2); // avoid to compute multiple times
    // Falling edge of the trapzoidal speed profile
    if (rho < params.d0) {
        v_ref = sqrt(2 * rho * params.acceleration) - rot_part;
    }
    
    // Rising edge of the trapzoidal speed profile
    else if ((travelled_distance < (params.d0)) && (v_ref < params.vMax - rot_part)){ 
        v_ref = sqrt(2 * (travelled_distance + 0.01) * params.acceleration) - rot_part;
    }

    // Constant speed phase of the trapzoidal speed profile
    else {
        v_ref = (params.vMax) - rot_part;
    }

    // print v_ref
    

    double ref_speed_left = (v_ref - distanceBetweenWheels * w_ref / 2) / wheel_radius;
    double ref_speed_right = (v_ref + distanceBetweenWheels * w_ref / 2) / wheel_radius;

    lowLevelController(ref_speed_left, ref_speed_right);
}