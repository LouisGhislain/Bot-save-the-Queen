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

    switch (STATE)
    {
    /*
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
    case 0: // MOVING TO FIRST STACK 
        robot->highLevelController(1, game);
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

    case 3: // FORWARD MANEUVER TO FIRST STACK
        robot->maneuver(0.14, game);
        STATE++;
        break;

    case 4: // MANEUVERING
        if (robot->end_of_manoeuvre){
            STATE++;
        }
        break;

    case 5: // GRABBING FIRST STACK
            robot->teensy_grab();
            STATE++;
        break;

    case 6: // MOVING TO BUILD ZONE
        robot->highLevelController(3, game);
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

    case 9: // BUILD FIRST STACK
        robot->teensy_build(game);
        STATE++;
        break;

    case 10: // BUILDING
        if (robot->build_finished){
            STATE++;
        }
    
    case 11: // SHUTTING DOWN
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