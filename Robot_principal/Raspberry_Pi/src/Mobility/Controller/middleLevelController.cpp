#include "../../../include/Robot.h"

// Global movement parameters
const MovementParams manoeuvre {
    false,  // activated_target_angle
    0,   // d0
    0.175,    // vMax
    8,     // wMax
    0.01    // stop_robot_distance
};

const MovementParams straight {
    false,  // activated_target_angle
    0, //0.02,   // d0
    0.175,   //0.175,    // vMax
    8,     // wMax      // dans la premiere démo triple étage -> wMax subi = 9.5
    0.01    // stop_robot_distance
};

const MovementParams deplacement {
    false,  // activated_target_angle
    0.30,   // d0
    0.6,    //0.7,    // vMax  --> à la roue : 0.7 / 0.0295 = 23.73 rad/s;  
    4,     // wMax
    0.01    // stop_robot_distance
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
        GLOBAL_rho = sqrt(pow(delta_x_target, 2) + pow(delta_y_target, 2));
        rho = GLOBAL_rho; // Update the class member if needed elsewhere
        params = GLOBAL_params;
    }

    // ORIENTATE FUNCTION
    if(params.activated_target_angle){
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

    // STOP THE ROBOT IF THE TARGET IS REACHED
    if ((rho < params.stop_robot_distance) && (!(params == deplacement) || last_step == true)) {
        {   
            std::lock_guard<std::mutex> lock(ref_speed_mutex);
            ref_speed_left = 0;
            ref_speed_right = 0;
            end_of_manoeuvre = true;
        }
        //std::cout << "Point reached" << std::endl;
        return;
        }
    else{
        end_of_manoeuvre = false;
    }

    // COMPUTE THE ANGLE ERROR
    double alpha =  atan2(delta_y_target, delta_x_target) - my_angle;
        // Normalize alpha to range [-π, π]
    while (alpha > M_PI) {
        alpha -= 2 * M_PI;
    }
    while (alpha <= -M_PI) {
        alpha += 2 * M_PI;
    }

    // CHECK IF WE NEED TO GO BACKWARDS
    // If the robot is in a manoeuvre or going straight, we go backwards when target angle is > 90° or < -90°
    if((params == manoeuvre) || (params == straight)){
        if (alpha > M_PI/2){
            alpha = alpha - M_PI;
            backwards = true;

        }
        else if(alpha < -M_PI/2){
            alpha = alpha + M_PI;
            backwards = true;
        }
        else{
            backwards = false;
        }
    }
    else{
        backwards = false;
    }


    // COMPUTING THE OMEGA REF
    double w_ref = KpAlpha * alpha; 
    w_ref = std::clamp(w_ref, -params.wMax, params.wMax);
    double rot_part = abs(distanceBetweenWheels * w_ref / 2); // To avoid multiple calculations



    bool mylast_step;
    {
        std::lock_guard<std::mutex> lock(flags);
        mylast_step = last_step;
    }
    double my_travelled_distance;
    {
        std::lock_guard<std::mutex> lock(travelled_distance_mutex);
        my_travelled_distance = travelled_distance;
    }
    double my_rho_to_goal;
    {
        std::lock_guard<std::mutex> lock(rho_to_goal_mutex);
        my_rho_to_goal = rho_to_goal;
    }
    // COMPUTING V_REF DEPENDING ON THE TRAPEZOIDAL SPEED PROFILE :
    //   ->  FALLING EDGE
    if ((my_rho_to_goal < params.d0)) {
        v_ref = sqrt(2 * my_rho_to_goal * params.acceleration) - rot_part;
        v_ref = std::max(v_ref, 0.0);
        // Print Rising edge and vref
    }
    
    //   ->  RISING EDGE
    else if (my_travelled_distance < (params.d0)){ 
        v_ref = sqrt(2 * (my_travelled_distance + 0.001) * params.acceleration) - rot_part;
        v_ref = std::max(v_ref, v_threshold_move);
    }

    //   ->  CONSTANT SPEED PHASE
    else {
        v_ref = (params.vMax) - rot_part;
    }


    // IF WE GO BACKWARDS, WE INVERT THE V_REF
    if(((params == manoeuvre) || (params==straight)) && backwards){
        v_ref = -v_ref;
    }

    // APPLY THE COMPUTED ref_speed TO THE WHEELS
    {
        std::lock_guard<std::mutex> lock(ref_speed_mutex);
        ref_speed_left = (v_ref - distanceBetweenWheels * w_ref / 2) / wheel_radius;
        ref_speed_right = (v_ref + distanceBetweenWheels * w_ref / 2) / wheel_radius;
    }
}