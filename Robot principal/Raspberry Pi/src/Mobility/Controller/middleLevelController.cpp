#include "../../../include/Robot.h"

// Global movement parameters
const MovementParams manoeuvre {
    false,  // activated_target_angle
    0.02,   // d0
    0.2,    // vMax
    0.03    // stop_robot_distance
};

const MovementParams straightMotion {    // FLORINAX C'EST POUR TOI (fonction pour séparer les stacks)
    false,  // activated_target_angle
    0.02,   // d0
    0.175,   //0.175,    // vMax
    9.5,     // wMax      // dans la premiere démo triple étage -> wMax subi = 9.5
    0.01    // stop_robot_distance
};

const MovementParams deplacement {
    false,  // activated_target_angle
    0.30,   // d0
    0.7,    // vMax
    9.5,     // wMax
    0.03    // stop_robot_distance
};

const MovementParams orientation {
    true,   // activated_target_angle
    0.005,  // d0
    0.3,    // vMax
    4,     // wMax
    1.0 * M_PI / 180   // robot stop angle limit in rad
};

/**
 * @brief Middle-level controller - Go to a position
 * 
 * This function implements a middle-level controller that creates a trajectory to a target position.
 * 
 */
void Robot::middleLevelController(void *game) {
    GAME * mygame = (GAME *)game;
    Queen * myqueen = mygame->queen;
    bool backwards = false;
    
    double rho, x_coord_target, y_coord_target, angle_target;
    double my_x, my_y, my_angle;

    {
        std::lock_guard<std::mutex> lock(myqueen->position_mutex);
        my_x = myqueen->cart_pos->x;
        my_y = myqueen->cart_pos->y;
        my_angle = myqueen->angle;
    }

    MovementParams params;
    {
        std::lock_guard<std::mutex> lock(coord_mutex);
        x_coord_target = GLOBAL_x_coord_target;
        y_coord_target = GLOBAL_y_coord_target; 
        angle_target = GLOBAL_angle_target;
        delta_x_target = x_coord_target - my_x; // in m
        delta_y_target = y_coord_target - my_y; // in m
        //fprintf(stderr, "my x, my y, target x, target y: %f %f %f %f\n", my_x, my_y, x_coord_target, y_coord_target);
        GLOBAL_rho = sqrt(pow(delta_x_target, 2) + pow(delta_y_target, 2));
        rho = GLOBAL_rho; // Update the class member if needed elsewhere
        //fprintf(stderr, "global rho = %f\n", GLOBAL_rho);
        params = GLOBAL_params;
    }

    if(params.activated_target_angle){ // only applicable in the orientation mode
        double error_angle = angle_target - my_angle;
        {
            std::lock_guard<std::mutex> lock(ref_speed_mutex);
            double w_ref = Kp_orientation * error_angle;
            w_ref = std::clamp(w_ref, -params.wMax, params.wMax);
            ref_speed_left = (-distanceBetweenWheels * w_ref / 2) / wheel_radius;
            ref_speed_right = (distanceBetweenWheels * w_ref / 2) / wheel_radius;
        }
        if (abs(error_angle) < params.stop_robot_distance){
            end_of_angle = true;
        }else{
            end_of_angle = false;
        }
        return;
    }

    // Stop the robot when it reaches the target
    if (rho < params.stop_robot_distance){
        {   
            std::lock_guard<std::mutex> lock(ref_speed_mutex);
            ref_speed_left = 0;
            ref_speed_right = 0;
            end_of_manoeuvre = true;
        }
        travelled_distance = 0;
        //std::cout << "Point reached" << std::endl;
        return;
        }
    else{
        end_of_manoeuvre = false;
    }

    double alpha =  atan2(delta_y_target, delta_x_target) - my_angle;
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
    w_ref = std::clamp(w_ref, -params.wMax, params.wMax);
    fprintf(stderr, "wref = %f\n", w_ref);
    double rot_part = abs(distanceBetweenWheels * w_ref / 2); // avoid to compute multiple times

    // travelled distance useful for rising edge
    travelled_distance += abs(((distl + distr)-(last_distl_middle + last_distr_middle))/2);
    last_distl_middle = distl;
    last_distr_middle = distr;

    bool mylast_step;
    {
        std::lock_guard<std::mutex> lock(flags);
        mylast_step = last_step;
    }
    // Falling edge of the trapzoidal speed profile
    if ((rho < params.d0) && (mylast_step ==  true)) {
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

    {
        std::lock_guard<std::mutex> lock(ref_speed_mutex);
        ref_speed_left = (v_ref - distanceBetweenWheels * w_ref / 2) / wheel_radius;
        ref_speed_right = (v_ref + distanceBetweenWheels * w_ref / 2) / wheel_radius;
    }
}