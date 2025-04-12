#include "../../../include/Robot.h"

// Define global movement parameters (must be defined in one .cpp file)
const MovementParams manoeuvre {
    false,   // activated_target_angle
    0.02,  // d0
    0.175,    // vMax
    0.01   // stop_robot_distance
};

const MovementParams deplacement {
    false,  // activated_target_angle
    0.30,  // d0
    0.6,    // vMax
    0.03   // stop_robot_distance
};

const MovementParams orientation {
    true,   // activated_target_angle
    0.005,  // d0
    0.3,    // vMax
    0.001   // stop_robot_distance
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
    
    double rho, x_coord_target, y_coord_target, goal_angle;
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
        goal_angle = GLOBAL_goal_angle;
        delta_x_target = x_coord_target - my_x; // in m
        delta_y_target = y_coord_target - my_y; // in m
        //fprintf(stderr, "my x, my y, target x, target y: %f %f %f %f\n", my_x, my_y, x_coord_target, y_coord_target);
        GLOBAL_rho = sqrt(pow(delta_x_target, 2) + pow(delta_y_target, 2));
        rho = GLOBAL_rho; // Update the class member if needed elsewhere
        //fprintf(stderr, "global rho = %f\n", GLOBAL_rho);
        params = GLOBAL_params;
    }

    // Stop the robot when it reaches the target
    if (rho < params.stop_robot_distance){
        if(params.activated_target_angle == 1){
            {
                std::lock_guard<std::mutex> lock(ref_speed_mutex);
                // as if lowLevelController(0, 0);
                ref_speed_left = 0;
                ref_speed_right = 0;
                end_of_manoeuvre = true;

                // POUR LE ANGLE CONTROLLER :
                // Il faudrait calculer le Kp alpha et le w_ref avant ce if
                // Et n'imposer que ca sur les roues avec (v_ref = 0)
                // ref_speed_left = w_ref blablabla
                // ref_speed_right = w_ref blablabla
                // Puis return et mettre un flag angle_reached à true quand alpha < qu'une certaine valeur (pour la FSM)
            }

            travelled_distance = 0;           // Move feature to high level controller when ready
            //std::cout << "Point reached (maybe not orientation) " << std::endl;
            return;
            //continue; // if the target is reached but the angle is not, continue to rotate 
        }
        else{
            {   
                std::lock_guard<std::mutex> lock(ref_speed_mutex);
                ref_speed_left = 0;
                ref_speed_right = 0;
                end_of_manoeuvre = true;
            }

            travelled_distance = 0;           // Move feature to high level controller when ready
            //std::cout << "Point reached" << std::endl;
            return;
        }
    }else{
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