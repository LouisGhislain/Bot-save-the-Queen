#include "../../../include/Robot.h"

void Robot::maneuver(double dist, void *game) {
    GAME * mygame = (GAME *)game;
    Queen * myqueen = mygame->queen;
    
    double my_x, my_y, my_angle;

    {
        std::lock_guard<std::mutex> lock(myqueen->position_mutex);
    }
    {
        std::lock_guard<std::mutex> lock(coord_mutex);
        GLOBAL_params = manoeuvre;
        GLOBAL_x_coord_target = my_x + dist * cos(my_angle);
        GLOBAL_y_coord_target = my_y + dist * sin(my_angle);
    }

}

