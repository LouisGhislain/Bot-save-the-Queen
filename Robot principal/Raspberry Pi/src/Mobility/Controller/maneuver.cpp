#include "../../../include/Robot.h"

void Robot::maneuver(double dist, void *game) {
    GAME * mygame = (GAME *)game;
    Queen * myqueen = mygame->queen;
    
    params = manoeuvre;
    x_coord_target = myqueen->cart_pos->x + dist * cos(myqueen->angle);
    y_coord_target = myqueen->cart_pos->y + dist * sin(myqueen->angle);
}

