#include "../../../include/Robot.h"


/*! \brief Maneuver
* This function implements a maneuvering behavior for the robot.
* 
*
* @param nodeIndex Index of the node to move to with maneuver settings
*/
void Robot::maneuver(int node, void *game) {
    GAME * mygame = (GAME *)game;
    MAP * mymap = mygame->map;
    
    {
        std::lock_guard<std::mutex> lock(coord_mutex);
        GLOBAL_params = manoeuvre;
        GLOBAL_x_coord_target = mymap->nodes[node].x; // x coordinate of the target node
        GLOBAL_y_coord_target = mymap->nodes[node].y; // y coordinate of the target node
        end_of_manoeuvre = false;
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
        end_of_angle = false;
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

    // Helper lambda to compute minimal angular difference (wrapped)
    auto angle_diff = [](double a, double b) {
        double diff = fmod(b - a + M_PI, 2 * M_PI);
        if (diff < 0) diff += 2 * M_PI;
        return diff - M_PI;
    };

    // Define cardinal directions
    double angles[4] = {0, M_PI / 2, M_PI, -M_PI / 2};

    // Find the closest cardinal direction
    double min_diff = fabs(angle_diff(my_angle, angles[0]));
    int closest_idx = 0;

    for (int i = 1; i < 4; i++) {
        double diff = fabs(angle_diff(my_angle, angles[i]));
        if (diff < min_diff) {
            min_diff = diff;
            closest_idx = i;
        }
    }

    my_angle = angles[closest_idx];

    {
        std::lock_guard<std::mutex> lock(coord_mutex);
        GLOBAL_params = straight;
        GLOBAL_x_coord_target = my_x + (dist * cos(my_angle));
        GLOBAL_y_coord_target = my_y + (dist * sin(my_angle));
        fprintf(stderr, "Straight motion to (%f, %f)\n", GLOBAL_x_coord_target, GLOBAL_y_coord_target);
    }
}

/*! \brief Straight motion libre
* Cette fonction implémente un mouvement en ligne droite libre dans la direction actuelle du robot.
* 
* @param dist Distance à parcourir dans la direction actuelle de l'angle du robot (en mètres, peut être négative)
*/
void Robot::straightMotionLibre(double dist, void *game) {
    GAME * mygame = (GAME *)game;
    Queen * myqueen = mygame->queen;

    double my_x, my_y, my_angle;

    {
        std::lock_guard<std::mutex> lock(myqueen->position_mutex);
        my_x = myqueen->cart_pos->x;
        my_y = myqueen->cart_pos->y;
        my_angle = myqueen->angle;  // On garde l'angle tel quel (pas de snapping)
    }

    {
        std::lock_guard<std::mutex> lock(coord_mutex);
        GLOBAL_params = straight;
        GLOBAL_x_coord_target = my_x + (dist * cos(my_angle));
        GLOBAL_y_coord_target = my_y + (dist * sin(my_angle));
        //fprintf(stderr, "Straight motion libre to (%f, %f) with angle %f\n", GLOBAL_x_coord_target, GLOBAL_y_coord_target, my_angle);
    }
}