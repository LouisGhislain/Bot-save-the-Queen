#include "../../include/FSM.h"
int STATE_GRABBING = 0;

void fsm_grab_stack(Robot *robot, GAME *game, int PRE_NODE, int NODE){
    robot->stack_grabbed = false ; 
    switch(STATE_GRABBING){

        case 0 : //PATH PLANNING TO STACK
            robot->highLevelController(PRE_NODE, game);
            if (robot->end_of_travel){
                STATE_GRABBING++;
            }
            break ;

        case 1 : //MANEUVER TO STACK
            robot->maneuver(NODE, game);
            STATE_GRABBING++;
            break ; 

        case 2 : //MANEUVRING
            if (robot->end_of_manoeuvre){
                STATE_GRABBING++;
            }
            break ;

        case 3 : //GRABBING FIRST STACK
            robot->teensy_grab();
            STATE_GRABBING++;
            break ;

        case 4 : //MANEUVER BACK TO PRE_NODE
            if(NODE==NODE_STACK_8||NODE==NODE_STACK_9){ //avoid to hit the stage
                robot->maneuver(PRE_NODE, game);    
            }
            STATE_GRABBING++;
            break;

        case 5 : //MANEUVRING
            if (robot->end_of_manoeuvre){
                STATE_GRABBING++;
            }
            break ;

        case 6 : 
            robot->stack_grabbed = true ; 
            STATE_GRABBING = 0;
            break;
    }
}