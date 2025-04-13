#include "../../include/FSM.h"

State STATE = MOVING_FIRST_STACK;


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
    case MOVING_FIRST_STACK:
        robot->highLevelController(1, game);
        if (robot->end_of_travel){
            STATE = FIRST_MANEUVER;
        }
        break;

    case FIRST_MANEUVER:
        usleep(4000000);
        robot->maneuver(0.14, game);
        STATE = GRABBING;
        break;

    case GRABBING:
        std::cout << "End of manoeuvre: " << robot->end_of_manoeuvre << std::endl;
        if (robot->end_of_manoeuvre){
            robot->teensy_send_command(0x02); // Grab and lift to be able to travel after
            STATE = STOPPED;
        }
        break;

    
    case STOPPED:
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