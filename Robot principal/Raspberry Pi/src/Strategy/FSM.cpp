#include "../../include/FSM.h"

int STATE = 0;
int STATE_RETURN_TO_BASE = 0;
int PRE_END_ZONE;
int END_ZONE;

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
    //std::cout << "Robot state: " << STATE << std::endl;
    /*
    switch(STATE){
    case 0 : 
        drop_banner(robot, game);
        if(robot->banner_dropped){ // WAIT DROP
            STATE++ ; 
        }
        break ; 
    case 1 : // GO TO FIRST STACK AND GRAB
        fsm_grab_stack(robot, game, PRE_NODE_BOTTOM_STACK_4, NODE_STACK_4);
        if(robot->stack_grabbed){ // WAIT GRAB
            STATE++;
        }
        break;

    case 2 : // GO TO BIG CONSTRUCTION ZONE AND BUILD
        fsm_build_stack(robot, game, PRE_CONSTRUCTION_YELLOW_1 , CONSTRUCTION_YELLOW_1);
        if(robot->stack_builded){
            STATE++;
        }
        break;
    case 3 : 
        fsm_build_romain(robot, game, PRE_NODE_STACK_0, NODE_STACK_0);
        if(robot->stack_builded){
            STATE++;
        }
        break ; 
    case 4 :
        usleep(1000);
        break ; 
    }
    */
   switch (STATE){
        case 0 : // DROP THE BANNER
            drop_banner(robot, game);
            if(robot->banner_dropped){ // WAIT DROP
                 STATE++ ; 
            }
            break ; 

        case 1 : // GO TO FIRST STACK AND GRAB
            fsm_grab_stack(robot, game, PRE_NODE_BOTTOM_STACK_4, NODE_STACK_4);
            if(robot->stack_grabbed){ // WAIT GRAB
                STATE++;
            }
            break;

        case 2 : // GO TO BIG CONSTRUCTION ZONE AND BUILD
            fsm_build_stack(robot, game, PRE_CONSTRUCTION_YELLOW_1 , CONSTRUCTION_YELLOW_1);
            if(robot->stack_builded){
                STATE++;
            }
            break;

        case 3 : // GO TO SECOND STACK AND GRAB
            fsm_grab_stack(robot, game, PRE_NODE_STACK_0, NODE_STACK_0);
            if(robot->stack_grabbed){ //WAIT GRAB
                STATE++;
            }
            break;

        case 4 : //GO TO LITTLE CONSTRUCTION ZONE AND BUILD
            fsm_build_stack(robot, game, PRE_CONSTRUCTION_YELLOW_0 , CONSTRUCTION_YELLOW_0);
            if(robot->stack_builded){//WAIT BUILD
                STATE++;
            }
            break;

        case 5 : //GO TO THIRD STACK AND GRAB
            fsm_grab_stack(robot, game, PRE_NODE_STACK_8, NODE_STACK_8);
            if(robot->stack_grabbed){
                STATE++;
            }
            break ; 

        case 6 : //BUILD THIRD STAGES
            fsm_build_normal_third_stage(robot, game, PRE_CONSTRUCTION_YELLOW_1, PRE_NODE_STACK_0, CONSTRUCTION_YELLOW_0, CONSTRUCTION_YELLOW_1, PRE_PRE_CONSTUCTION_YELLOW_1);
            if(robot->stack_builded){
                STATE++;
            }
            break ; 

        case 7 : //RETURN TO BASE
            return_to_base(robot, game);
            break ;

        // ENDING IN RETURN TO BASE
        
        default:
            break;
    }
}

void start_from_yellow_side(Robot *robot, GAME *game){
    switch (STATE){

        case 1 : // DROP THE BANNER
            //drop_banner(robot, game);
            // if(robot->banner_dropped){ // WAIT DROP
            //      STATE++ ; 
            // }
            STATE++;
            break ; 
    }
}

void choose_start(Robot *robot, GAME *game){

    if(robot->avoidance_loop_activated){
        return; // do nothing is we are avoiding an ennemy
    }

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
            start_from_yellow_side(robot, game);
            //start_from_yellow_bottom(robot, game);
            break;
        default:
            fprintf(stderr, "No starting position precised, do it !");
            break;
    }
    
}

void return_to_base(Robot *robot, GAME *game){
    switch (STATE_RETURN_TO_BASE){

        case 0: // GO TO BASE NODE
            if (robot->starting_pos == 0 || robot->starting_pos == 1){ // in blue team
                PRE_END_ZONE = PRE_END_ZONE_BLUE;
                END_ZONE = END_ZONE_BLUE;
            }
            else if (robot->starting_pos == 2 || robot->starting_pos == 3){ // in yellow team
                PRE_END_ZONE = PRE_END_ZONE_YELLOW;
                END_ZONE = END_ZONE_YELLOW;
            }
            robot->highLevelController(PRE_END_ZONE, game);
            if (robot->end_of_travel){
                STATE_RETURN_TO_BASE++;
            }
            break;

        case 1 : //ORIENTATE TO BASE
            robot->points_scored += 10 ;
            robot->orientate(90, game);
            STATE_RETURN_TO_BASE++;
            break;

        case 2: // ORIENTATING 
            if (robot->end_of_angle){
                STATE_RETURN_TO_BASE++;
            }
            robot->screen_end_game(); //show the score on the screen
            break;

        case 3 : // MANEUVER TO END ZONE   
            if(get_match_time(game) > time_reach_end_zone){
                robot->maneuver(END_ZONE, game);
                STATE_RETURN_TO_BASE++;
            }
            break; 

        case 4 : // MANEUVERING
            if(robot->end_of_manoeuvre){
                STATE_RETURN_TO_BASE++;
            }
            break; 

        case 5: // FINISHED MATCH
            fprintf(stderr, "FINISHED MATCH\n");
            //robot->buzzBuzzer();
            STATE_RETURN_TO_BASE++;
            break;

        case 6: // SHUTTING DOWN
            usleep(0.001*1000000);
            break;

    }
}