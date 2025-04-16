#include "../../include/Robot.h"

void Robot::stop_if_ennemy(){
    // we want to stop the robot in 0.5 seconds when being at max angular speed (23.73 rad/s)
    // so the decreasing speed step is 23.73 / (number_of_iterations_to_stop) rad/s
    // = 23.73 / (0.5 sec / 0.010 sec) rad/s = 23.73 / 50 rad/s = 0.4746 rad/s
    // with number_of_iterations_to_stop = 0.5 sec / 0.010 sec = 50 iterations (0.010 is the loop time)
    {
        std::lock_guard<std::mutex> lock(ref_speed_mutex);
        double next_ref_speed_left = ref_speed_left - 0.4746;
        double next_ref_speed_right = ref_speed_right - 0.4746;
        if (next_ref_speed_left < 0) {
            next_ref_speed_left = 0;
        }
        if (next_ref_speed_right < 0) {
            next_ref_speed_right = 0;
        }
        ref_speed_left = next_ref_speed_left;
        ref_speed_right = next_ref_speed_right;

        fprintf(stderr, "ref speed left: %f, ref speed right: %f\n", ref_speed_left, ref_speed_right);
    }
}

std::pair<double, double> Robot::get_distance_to_ennemy(GAME *game){
    double distance_to_ennemy;
    double angle_to_ennemy;
    double sauron_x, sauron_y;
    {
        std::lock_guard<std::mutex> lock(game->Sauron->sauron_mutex);
        sauron_x = game->Sauron->cart_pos->x/1000; // get the position of the ennemy in meter
        sauron_y = game->Sauron->cart_pos->y/1000;
        //fprintf(stderr, "ennemy position: x : %f, y : %f\n", game->Sauron->cart_pos->x, game->Sauron->cart_pos->y);
    }

    {
        std::lock_guard<std::mutex> lock(game->queen->position_mutex);
        distance_to_ennemy = sqrt(pow((sauron_x - game->queen->cart_pos->x),2) + pow((sauron_y - game->queen->cart_pos->y),2));  
        angle_to_ennemy = atan2(sauron_y - game->queen->cart_pos->y, sauron_x - game->queen->cart_pos->x) - game->queen->angle;
    }
    angle_to_ennemy = abs(angle_to_ennemy * 180 / M_PI); // convert to degrees
    while (angle_to_ennemy > 180) {
        angle_to_ennemy -= 360;
    }
    angle_to_ennemy = abs(angle_to_ennemy); // get the absolute value of the angle
    //fprintf(stderr, "ennemy position: x: %f,               y: %f\n", sauron_x, sauron_y);
    //fprintf(stderr, "distance to ennemy: %f, angle to ennemy: %f\n", distance_to_ennemy, angle_to_ennemy);
    
    return {distance_to_ennemy, angle_to_ennemy};
} 