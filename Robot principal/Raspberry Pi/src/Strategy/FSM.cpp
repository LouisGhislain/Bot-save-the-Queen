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
    /*
    case 0: // MOVING TO FIRST STACK 
        robot->highLevelController(0, game);
        if (robot->end_of_travel){
            STATE++;
        }
        break;

    case 1: // DEFINE ORIENTATION TO FIRST STACK
        robot->orientate(90, game);
        STATE++;
        break;

    case 2: // ORIENTATING
        if (robot->end_of_angle){
            STATE++;
        }
        break;

    case 3: // FORWARD straightMotion TO FIRST STACK
        robot->straightMotion(0.18, game);
        STATE++;
        break;

    case 4: // straightMotionING
        if (robot->end_of_manoeuvre){
            STATE++;
        }
        break;

    case 5: // GRABBING FIRST STACK
            robot->teensy_grab();
            STATE++;
        break;

    case 6: // MOVING TO BUILD ZONE
        robot->highLevelController(2, game);
        if (robot->end_of_travel){
            STATE++;
        }
        break;

    case 7: // DEFINE ORIENTATION TO BUILD ZONE
        robot->orientate(-90, game);
        STATE++;
        break;

    case 8: // ORIENTATING
        if (robot->end_of_angle){
            STATE++;
        }
        break;

    case 9: // FORWARD straightMotion TO BUILD ZONE
        robot->straightMotion(0.13, game);
        STATE++;
        break;

    case 10: // straightMotionING
        if (robot->end_of_manoeuvre){
            STATE++;
        }
        break;

    case 11: // BUILD FIRST STACK
        robot->teensy_build(game);
        STATE++;
        break;

    case 12: // BUILDING
        if (robot->build_finished){
            STATE++;
        }

    case 13: // BACKWARD straightMotion AFTER BUILDING
        robot->straightMotion(-0.15, game);
        STATE++;
        break;
    
    case 14: // straightMotionING
        if (robot->end_of_manoeuvre){
            STATE++;
        }
        break;

    case 15: // MOVING TO SECOND STACK
        robot->highLevelController(4, game);
        if (robot->end_of_travel){
            STATE++;
        }
        break;

    case 16: // DEFINE ORIENTATION TO SECOND STACK
        robot->orientate(-90, game);
        STATE++;
        break;

    case 17: // ORIENTATING
        if (robot->end_of_angle){
            STATE++;
        }
        break;

    case 18: // FORWARD straightMotion TO SECOND STACK
        robot->straightMotion(0.18, game);
        STATE++;
        break;

    case 19: // straightMotionING
        if (robot->end_of_manoeuvre){
            STATE++;
        }
        break;

    case 20: // GRABBING SECOND STACK
        robot->teensy_grab();
        STATE++;
        break;

    case 21: // MOVING TO BUILD ZONE
        robot->highLevelController(2, game);
        if (robot->end_of_travel){
            STATE++;
        }
        break;

    case 22: // DEFINE ORIENTATION TO BUILD ZONE
        robot->orientate(-90, game);
        STATE++;
        break;

    case 23: // ORIENTATING
        if (robot->end_of_angle){
            STATE++;
        }
        break;
    
    case 24: // BUILD SECOND STACK
        robot->teensy_americain_third_stage(game);
        STATE++;
        break;

    case 25: // BUILDING
        if (robot->build_finished){
            STATE++;
        }
        break;

    case 26: // BACKWARD straightMotion AFTER BUILDING
        robot->straightMotion(-0.15, game);
        STATE++;
        break;

    case 27: // straightMotionING
        if (robot->end_of_manoeuvre){
            STATE++;
        }
        break;

    case 28: // SHUTTING DOWN
        robot->buzzBuzzer();
        usleep(500);
        STATE++;
    case 29:
        usleep(30000);
        break;
        
    default:
        break;
    */
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