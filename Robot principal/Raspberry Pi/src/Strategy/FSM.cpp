#include "../../include/FSM.h"

int STATE = 0;
int STATE_RETURN_TO_BASE = 0;

void start_from_blue_bottom(Robot *robot, GAME *game){
    // pour le moment, on fait circuler le robot autour de la map
    // c'est uniquement des test, en aucun cas c'est la stratégie 
    switch (STATE){

        case 0 : // go to node 36 23 37 22
            robot->highLevelController(36, game);
            if (robot->end_of_travel){
                STATE++;
            }
            break ;
        case 1 : // 
            robot->highLevelController(23, game);
            if (robot->end_of_travel){
                STATE++;
            }
            break ;
        case 2 : // 
            robot->highLevelController(37, game);
            if (robot->end_of_travel){
                STATE++;
            }
            break ;
        case 3 : // 
            robot->highLevelController(22, game);
            if (robot->end_of_travel){
                STATE = 0;
            }
            break;
    }
}

void start_from_blue_side(Robot *robot, GAME *game){
    return;
}

void start_from_yellow_bottom(Robot *robot, GAME *game){
    // Print robot state
    std::cout << "Robot state: " << STATE << std::endl;

    switch (STATE){

        case 0 : // DROP THE BANNER
            // drop_banner(robot, game);
            // if(robot->banner_dropped){ // WAIT DROP
            //     STATE++ ; 
            // }
            STATE++;
            break ; 

        case 1 : // GO TO STACK AND GRAB
            fsm_grab_stack(robot, game, PRE_NODE_BOTTOM_STACK_4, NODE_STACK_4);
            if(robot->stack_grabbed){ // WAIT GRAB
                STATE++;
            }
            break;

        case 2 : // GO TO CONSTRUCTION ZONE AND BUILD
            fsm_build_stack(robot, game, PRE_CONSTRUCTION_YELLOW_1 , CONSTRUCTION_YELLOW_1);
            if(robot->stack_builded){
                STATE++;
            }
            break;

        case 3 : // GO TO STACK AND GRAB
            fsm_grab_stack(robot, game, PRE_NODE_STACK_0, NODE_STACK_0);
            // Print stack_grabbed
            std::cout << "stack_grabbed: " << robot->stack_grabbed << std::endl;
            if(robot->stack_grabbed){ //WAIT GRAB
                STATE++;
            }
            break;

        case 4 : //GO BUILD AMERICAIN STAGE
            fsm_build_american_stage(robot, game, PRE_CONSTRUCTION_YELLOW_1 , CONSTRUCTION_YELLOW_1);
            if(robot->stack_builded){//WAIT BUILD
                STATE++;
            }
            break;

        case 5 : //BUZZ
            robot->buzzBuzzer();
            usleep(500);
            STATE++;
            break;
            
        case 6 : //SHUTTING DOWN
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

    if(get_match_time(game) > time_return_to_base){
        return_to_base(robot, game);
        return;
    }

    switch (robot->starting_pos)
    {
        case 0: // Blue bottom
            start_from_blue_bottom(robot, game);
            //start_from_yellow_bottom(robot, game);
            break;
        case 1: // Blue side
            //start_from_blue_side(robot, game);
            start_from_yellow_bottom(robot, game);
            break;
        case 2: // Yellow bottom
            start_from_yellow_bottom(robot, game);
            break;
        case 3: // Yellow side
            //start_from_yellow_side(robot, game);
            start_from_yellow_bottom(robot, game);
            break;
        default:
            fprintf(stderr, "No starting position precised, do it !");
            break;
    }
    
}

void return_to_base(Robot *robot, GAME *game){
    if (robot->starting_pos == 0 || robot->starting_pos == 1){ // in blue team
        return_to_base_blue(robot, game);
    }
    else if (robot->starting_pos == 2 || robot->starting_pos == 3){ // in yellow team
        return_to_base_yellow(robot, game);
    }
}

void return_to_base_blue(Robot *robot, GAME *game){
switch (STATE_RETURN_TO_BASE){
    case 0: // return to base
        robot->highLevelController(END_ZONE_BLUE, game);
        if (robot->end_of_travel){
            STATE_RETURN_TO_BASE++;
        }
        break;
    case 1: // extend crémaillère
        // TODO // extend crémaillère
        STATE_RETURN_TO_BASE++;
        break;
    case 2: // FINISHED MATCH
        // print the score on the screen
        fprintf(stderr, "FINISHED MATCH\n");
        robot->buzzBuzzer();
        STATE_RETURN_TO_BASE++;
        break;
    case 3: // SHUTTING DOWN
        usleep(100000);
        break;
}


}

void return_to_base_yellow(Robot *robot, GAME *game){
    switch (STATE_RETURN_TO_BASE){
        case 0: // return to base
            robot->highLevelController(END_ZONE_YELLOW, game);
            if (robot->end_of_travel){
                STATE_RETURN_TO_BASE++;
            }
            break;
        case 1: // extend crémaillère
            // TODO // extend crémaillère
            STATE_RETURN_TO_BASE++;
            break;
        case 2: // FINISHED MATCH
            // print the score on the screen
            fprintf(stderr, "FINISHED MATCH\n");
            robot->buzzBuzzer();
            STATE_RETURN_TO_BASE++;
            break;
        case 3: // SHUTTING DOWN
            usleep(100000);
            break;
        }
}