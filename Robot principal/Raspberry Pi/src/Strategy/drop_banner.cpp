#include "../../include/FSM.h"
int STATE_DROPPING = 0 ;
bool banner_dropped = false; 

void drop_banner(Robot *robot, GAME *game){
    banner_dropped = false ;
    switch(STATE_DROPPING){
    case 0 : //backward to drop 
        robot->straightMotion(-0.045, game);
        STATE_DROPPING++;
        break;
    case 1 : 
        if (robot->end_of_manoeuvre){
            usleep(100000);
            STATE_DROPPING++;
        }
        break;
    case 2 : 
        robot->straightMotion(0.045,game);
        STATE_DROPPING++;
        break ; 
    case 3 : 
        if(robot->end_of_manoeuvre){
            banner_dropped = true ; 
            STATE_DROPPING = 0 ;
        }
    }
}