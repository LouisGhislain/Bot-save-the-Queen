#include "../../include/FSM.h"
int STATE_BUILDING = 0;

void fsm_build_stack(Robot *robot, GAME *game, int PRE_NODE, int NODE){
    robot->stack_builded = false ; 
    switch(STATE_BUILDING){

        case 0 : //PATH PLANNING TO PRE_NODE BUILDING ZONE
            robot->highLevelController(PRE_NODE, game);
            if (robot->end_of_travel){
                STATE_BUILDING++;
            }
        break;

        case 1 : //MANEUVER TO NODE BUILDING ZONE
            robot->maneuver(NODE, game);
            STATE_BUILDING++;
            break ;

        case 2 : //MANEUVRING
            if (robot->end_of_manoeuvre){
                STATE_BUILDING++;
            }
            break ;

        case 3 : //BUILD STACK
            robot->teensy_build(game);
            STATE_BUILDING++;
            break ;

        case 4 : //BUILDING
            if (robot->build_finished){
                STATE_BUILDING++;
            }
            break ;

        case 5 : //BACKWARD MANEUVER AFTER BUILDING
            robot->maneuver(PRE_NODE, game);
            STATE_BUILDING++;
            break;
            
        case 6 : 
            if (robot->end_of_manoeuvre){
                robot->stack_builded = true ; 
                STATE_BUILDING = 0 ;
            }
            break;
    }
}


void fsm_build_american_stage(Robot *robot, GAME *game, int PRE_NODE, int NODE){
    robot->stack_builded = false ; 
    switch(STATE_BUILDING){
        case 0 : //MOVING TO STACK
            robot->highLevelController(PRE_NODE, game);
            if (robot->end_of_travel){
                STATE_BUILDING++;
            }
            break ; 
        case 1 : //ORIENTING TO BUILD ZONE
            robot->orientate(-90, game);
            if (robot->end_of_angle){
                STATE_BUILDING++;
            }
            break;
        case 2 : //BUILD AMERICAIN STAGE
            robot->teensy_americain_third_stage(NODE, game);
            STATE_BUILDING++;
            break ; 
        case 3 : //BUILDING
            if (robot->build_finished){
                STATE_BUILDING++;
            }
            break ; 
        case 4 : // BACKWARD MANEUVER AFTER BUILDING
            robot->maneuver(PRE_NODE, game);            
            STATE_BUILDING++;
            break;
        case 5 : //MANEUVRING
            if (robot->end_of_manoeuvre){
                robot->stack_builded = true ; 
                STATE_BUILDING=0;
            }
            break;
    }
}
    
/* TEST SEPARATING
    case GRABBING : 
        robot->teensy_send_command(0x02); // Grab
        STATE = WAITING ; 
        break ; 
    case WAITING : 
        usleep(3000000);
        STATE = SEPARATING ;
        break ;
    case SEPARATING : 
        robot->teensy_build(game);
        STATE = STOPPED ; 
        break; 
    */