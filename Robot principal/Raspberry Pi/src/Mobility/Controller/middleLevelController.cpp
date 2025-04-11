#include "../../../include/Robot.h"

// Define global movement parameters (must be defined in one .cpp file)
const MovementParams manoeuvre {
    true,   // activated_target_angle
    0.30,  // d0
    0.6,    // vMax
    0.04   // stop_robot_distance
};

const MovementParams deplacement {
    false,  // activated_target_angle
    0.30,  // d0
    0.6,    // vMax
    0.04   // stop_robot_distance
};

const MovementParams orientation {
    true,   // activated_target_angle
    0.005,  // d0
    0.0,    // vMax
    0.001   // stop_robot_distance
};

/**
 * @brief Middle-level controller - Go to a position
 * 
 * This function implements a middle-level controller that creates a trajectory to a target position.
 * 
 * @param x X-coordinate of the target position
 * @param y Y-coordinate of the target position
 * @param goal_angle Angle to the target position (if not important, put anything)
 * @param params Movement parameters
 * @param game Pointer to the game structure
 */
void Robot::middleLevelController(void *game) {
    GAME * mygame = (GAME *)game;
    Queen * myqueen = mygame->queen;
    bool backwards = false;
    
    // delta to the target
    delta_x_target = x_coord_target - myqueen->cart_pos->x; // in m
    delta_y_target = y_coord_target - myqueen->cart_pos->y; // in m

    travelled_distance += abs(((distl + distr)-(last_distl_middle + last_distr_middle))/2);
    rho = sqrt(pow(delta_x_target, 2) + pow(delta_y_target, 2)); 

    last_distl_middle = distl;
    last_distr_middle = distr;

    // Stop the robot when it reaches the target
    if (rho < params.stop_robot_distance){
        if(params.activated_target_angle == 1){
            
            // as if lowLevelController(0, 0);
            ref_speed_left = 0;
            ref_speed_right = 0;

            travelled_distance = 0;           // Move feature to high level controller when ready
            //std::cout << "Point reached (maybe not orientation) " << std::endl;
            return;
            //continue; // if the target is reached but the angle is not, continue to rotate 
        }
        else{
            // as if lowLevelController(0, 0);
            ref_speed_left = 0;
            ref_speed_right = 0;

            travelled_distance = 0;           // Move feature to high level controller when ready
            //std::cout << "Point reached" << std::endl;
            return;
        }
    }

    double alpha =  atan2(delta_y_target, delta_x_target) - myqueen->angle;
    // Normalize alpha to range [-π, π]
    while (alpha > M_PI) {
        alpha -= 2 * M_PI;
    }
    while (alpha <= -M_PI) {
        alpha += 2 * M_PI;
    }

    // If backwards
    if(params == manoeuvre){
        if (alpha > M_PI/2){
            alpha = alpha - M_PI;
            backwards = true;

        }
        if(alpha < -M_PI/2){
            alpha = alpha + M_PI;
            backwards = true;
        }
    }

    double w_ref = KpAlpha * alpha;  // attention sinus pourrait apporter de la stabilité

    double rot_part = abs(distanceBetweenWheels * w_ref / 2); // avoid to compute multiple times

    // Falling edge of the trapzoidal speed profile
    if (rho < params.d0) {
        v_ref = sqrt(2 * rho * params.acceleration) - rot_part;
        //fprintf(stderr, "FALLING EDGE =%f\n", v_ref);
    }
    
    // Rising edge of the trapzoidal speed profile
    else if (travelled_distance < (params.d0)){ 
        v_ref = sqrt(2 * (travelled_distance + 0.001) * params.acceleration) - rot_part;
        v_ref = std::max(v_ref, v_threshold_move);
        //fprintf(stderr, "RISING EDGE vref =%f\n", v_ref);
    }

    // Constant speed phase of the trapzoidal speed profile
    else {
        v_ref = (params.vMax) - rot_part;
        //fprintf(stderr, "CONSTANT SPEED =%f\n", v_ref);
    }

    // If backwards
    if(params == manoeuvre && backwards){
        v_ref = -v_ref;
    }

    ref_speed_left = (v_ref - distanceBetweenWheels * w_ref / 2) / wheel_radius;
    ref_speed_right = (v_ref + distanceBetweenWheels * w_ref / 2) / wheel_radius;
}