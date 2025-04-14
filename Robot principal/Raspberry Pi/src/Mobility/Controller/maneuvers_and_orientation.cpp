#include "../../../include/Robot.h"


/*! \brief Maneuver
* This function implements a maneuvering behavior for the robot.
* 
*
* @param node Node to move to with maneuver settings
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
}

/*! \brief Orientate
* This function implements a rotation behavior for the robot.
*
* @param angle Angle to rotate in degrees, can be negative
*/
void Robot::orientate(double angle, void *game) {
    GAME * mygame = (GAME *)game;
    {
        std::lock_guard<std::mutex> lock(coord_mutex);
        GLOBAL_params = orientation;
        GLOBAL_angle_target = (angle/180) * M_PI; // Convert to radians
    }
}

/*! \brief Straight motion
* This function implements a straight motion behavior for the robot.
* 
*
* @param dist Distance to move in the direction of the robot's current angle in meters, can be negative
*/
void Robot::straightMotion(double dist, void *game) {
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
}