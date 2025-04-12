#include "../../../include/Robot.h"


/*! \brief Maneuver
* This function implements a maneuvering behavior for the robot.
* 
*
* @param dist Distance to move in the direction of the robot's current angle in meters, can be negative
*/
void Robot::maneuver(double dist, void *game) {
    GAME * mygame = (GAME *)game;
    Queen * myqueen = mygame->queen;
    
    double my_x, my_y, my_angle;

    {
        std::lock_guard<std::mutex> lock(myqueen->position_mutex);
        my_x = myqueen->cart_pos->x;
        my_y = myqueen->cart_pos->y;
        my_angle = myqueen->angle;
    }
    {
        std::lock_guard<std::mutex> lock(coord_mutex);
        GLOBAL_params = manoeuvre;
        GLOBAL_x_coord_target = my_x + (dist * cos(my_angle));
        GLOBAL_y_coord_target = my_y + (dist * sin(my_angle));
    }
    // Print dist * cos(my_angle) and dist * sin(my_angle)
    std::cout << "Dist" << dist << std::endl;
    std::cout << "Angle: " << my_angle << std::endl;
    std::cout << "Displacement: " << dist * cos(my_angle) << ", " << dist * sin(my_angle) << std::endl;
}

