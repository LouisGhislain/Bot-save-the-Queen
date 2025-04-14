#include "../../include/FSM.h"
int STATE = 0;

void start_from_blue_bottom(Robot *robot, GAME *game){
    return;
}

void start_from_blue_side(Robot *robot, GAME *game){
    return;
}

void start_from_yellow_bottom(Robot *robot, GAME *game){
    // Print robot state
    std::cout << "Robot state: " << STATE << std::endl;

    switch (STATE){
    /*TEST BANNER
    case 0 : 
        robot->straightMotion(-0.045, game);
        STATE++;
        break;
    case 1 : 
        if (robot->end_of_manoeuvre){
            usleep(100000);
            STATE++;
        }
        break;
    
    case 2 : 
        robot->highLevelController(0, game);
        if (robot->end_of_travel){
            STATE++;
        }
        break;
    
    case 3:
        usleep(30000);
        break;
        
    default:
        break;
    /*
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

    //3stages
    
    case 0: // MOVING TO FIRST STACK 
        robot->highLevelController(PRE_NODE_STACK_4, game);
        if (robot->end_of_travel){
            STATE++;
        }
        break;

    case 1: // MANEUVER TO FIRST STACK
        robot->maneuver(NODE_STACK_4, game);            // SPECIFY NODE
        STATE++;
        break;

    case 2: // MANEUVERING
        if (robot->end_of_manoeuvre){
            STATE++;
        }
        break;

    case 3: // GRABBING FIRST STACK
            robot->teensy_grab();
            STATE++;
        break;

    case 4: // MOVING TO BUILD ZONE
        robot->highLevelController(PRE_CONSTRUCTION_YELLOW_1, game);
        if (robot->end_of_travel){
            STATE++;
        }
        break;

    case 5: // MANEUVER TO BUILD ZONE
        robot->maneuver(CONSTRUCTION_YELLOW_1, game);            // SPECIFY NODE
        STATE++;
        break;

    case 6: // MANEUVERING
        if (robot->end_of_manoeuvre){
            STATE++;
        }
        break;

    case 7: // BUILD FIRST STACK
        robot->teensy_build(game);
        STATE++;
        break;

    case 8: // BUILDING
        if (robot->build_finished){
            STATE++;
        }

    case 9: // BACKWARD MANEUVER AFTER BUILDING
        robot->maneuver(PRE_CONSTRUCTION_YELLOW_1, game);            // SPECIFY NODE
        STATE++;
        break;
    
    case 10: // MANEUVERING
        if (robot->end_of_manoeuvre){
            STATE++;
        }
        break;

    case 11: // MOVING TO SECOND STACK
        robot->highLevelController(PRE_NODE_STACK_0, game);
        if (robot->end_of_travel){
            STATE++;
        }
        break;

    case 12: // FORWARD MANEUVER TO SECOND STACK
        robot->maneuver(NODE_STACK_0, game);            // SPECIFY NODE
        STATE++;
        break;

    case 13: // MANEUVERING
        if (robot->end_of_manoeuvre){
            STATE++;
        }
        break;

    case 14: // GRABBING SECOND STACK
        robot->teensy_grab();
        STATE++;
        break;

    case 15: // MOVING TO BUILD ZONE
        robot->highLevelController(PRE_CONSTRUCTION_YELLOW_1, game);
        if (robot->end_of_travel){
            STATE++;
        }
        break;

    case 16: // ORIENTING TO BUILD ZONE
        robot->orientate(-90, game);
        if (robot->end_of_angle){
            STATE++;
        }
        break;
    
    case 17: // BUILD SECOND STACK
        robot->teensy_americain_third_stage(5, game);
        STATE++;
        break;

    case 18: // BUILDING
        if (robot->build_finished){
            STATE++;
        }
        break;

    case 19: // BACKWARD MANEUVER AFTER BUILDING
        robot->maneuver(PRE_CONSTRUCTION_YELLOW_1, game);            // SPECIFY NODE
        STATE++;
        break;

    case 20: // MANEUVERING
        if (robot->end_of_manoeuvre){
            STATE++;
        }
        break;

    case 21: // SHUTTING DOWN
        robot->buzzBuzzer();
        usleep(500);
        STATE++;

    case 22: // SHUTTING DOWN
        usleep(30000);
        break;
        
    default:
        break;
    
    }
}

void start_from_yellow_side(Robot *robot, GAME *game){
    return;
}

void choose_start(Robot *robot, GAME *game){
    switch (robot->starting_pos)
    {
        case 0: // Blue bottom
            start_from_blue_bottom(robot, game);
            break;
        case 1: // Blue side
            start_from_blue_side(robot, game);
            break;
        case 2: // Yellow bottom
            start_from_yellow_bottom(robot, game);
            break;
        case 3: // Yellow side
            start_from_yellow_side(robot, game);
            break;
        default:
            fprintf(stderr, "No starting position precised, do it !");
            break;
    }
}