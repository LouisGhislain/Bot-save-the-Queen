#include "../../include/FSM.h"

int STATE = 0;
int STATE_RETURN_TO_BASE = 0;
int PRE_END_ZONE;
int END_ZONE;
bool match_finished = false;

void start_from_blue_bottom_adversary_bottom(Robot *robot, GAME *game){

    
    switch (STATE){

        case 0 : // DROP THE BANNER
            drop_banner(robot, game);
            if(robot->banner_dropped){ // WAIT DROP
                 STATE++ ; 
            }
            break ; 

        case 1 : // GO TO FIRST STACK AND GRAB
            fsm_grab_stack(robot, game, PRE_NODE_BOTTOM_STACK_5, NODE_STACK_5);
            if(robot->stack_grabbed){ // WAIT GRAB
                STATE++;
                usleep(0.5*1000000);
            }
            break;

        case 2 : // GO TO BIG CONSTRUCTION ZONE AND BUILD
            fsm_build_stack(robot, game, PRE_CONSTRUCTION_BLUE_1 , CONSTRUCTION_BLUE_1);
            if(robot->stack_builded){
                STATE++;
            }
            break;

        case 3 : // GO TO SECOND STACK AND GRAB
            fsm_grab_stack(robot, game, PRE_NODE_STACK_1, NODE_STACK_1);
            if(robot->stack_grabbed){ //WAIT GRAB
                STATE++;
            }
            break;

        case 4 : //GO TO LITTLE CONSTRUCTION ZONE AND BUILD
            fsm_build_stack(robot, game, PRE_CONSTRUCTION_BLUE_0 , CONSTRUCTION_BLUE_0);
            if(robot->stack_builded){//WAIT BUILD
                STATE++;
            }
            break;

        case 5 : //GO TO THIRD STACK AND GRAB
            fsm_grab_stack(robot, game, PRE_NODE_STACK_9, NODE_STACK_9);
            if(robot->stack_grabbed){
                STATE++;
            }
            break ; 

        case 6 : //BUILD THIRD STAGES
            fsm_build_normal_third_stage(robot, game, PRE_CONSTRUCTION_BLUE_1, PRE_NODE_STACK_1, CONSTRUCTION_BLUE_0, CONSTRUCTION_BLUE_1, PRE_PRE_CONSTUCTION_BLUE_1);
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

void start_from_blue_bottom_adversary_side(Robot *robot, GAME *game){
        switch(STATE){
        case 0 : // DROP THE BANNER
            drop_banner(robot, game);
            if(robot->banner_dropped){ // WAIT DROP
                 STATE++ ; 
            }
            break ; 

        case 1 : 
            fsm_grab_stack(robot, game, PRE_NODE_BOTTOM_STACK_5, NODE_STACK_5);
            if(robot->stack_grabbed){
                STATE++;
            }
            break ;


        case 2 : 
            fsm_build_stack(robot, game, PRE_CONSTRUCTION_BLUE_3, CONSTRUCTION_BLUE_3);
            if(robot->stack_builded){
                STATE++;
            }
            break;
        
        case 3 : 
            fsm_grab_stack(robot, game, PRE_NODE_STACK_2, NODE_STACK_2);
            if(robot->stack_grabbed){
                STATE++;
            }
            break; 

        case 4 : 
            fsm_build_stack(robot, game, PRE_CONSTRUCTION_BLUE_2, CONSTRUCTION_BLUE_2);
            if(robot->stack_builded){
                STATE++;
            }
            break;
        case 5 : 
            fsm_grab_stack(robot, game, PRE_NODE_STACK_6, NODE_STACK_6);
            if(robot->stack_grabbed){
                STATE++;
            }
            break ; 

        case 6 : 
            fsm_build_normal_third_stage(robot, game, PRE_CONSTRUCTION_BLUE_3, PRE_CONSTRUCTION_BLUE_2, CONSTRUCTION_BLUE_2, CONSTRUCTION_BLUE_3, 36);
            if(robot->stack_builded){
                STATE++;
            }
            break ; 
        case 7 : 
            break ; 
        default : 
            break ; 
    }
}

void start_from_blue_side(Robot *robot, GAME *game){
    //to do 
}

void start_from_yellow_bottom_adversary_bottom(Robot *robot, GAME *game){
   switch (STATE){
        case 0 : // DROP THE BANNER
            drop_banner(robot, game);
            if(robot->banner_dropped){ // WAIT DROP
                 STATE++ ; 
            }
            break ; 
        
        case 1 : 
            fsm_grab_stack(robot, game, PRE_NODE_BOTTOM_STACK_4, NODE_STACK_4);
            if(robot->stack_grabbed){
                STATE++ ; 
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

void start_from_yellow_bottom_adversary_side(Robot *robot, GAME *game){
    switch(STATE){

        case 0 : // DROP THE BANNER
            drop_banner(robot, game);
            if(robot->banner_dropped){ // WAIT DROP
                 STATE++ ; 
            }
            break ; 

        case 1 : 
            fsm_grab_stack(robot, game, PRE_NODE_BOTTOM_STACK_4, NODE_STACK_4);
            if(robot->stack_grabbed){
                STATE++;
            }
            break ;


        case 2 : 
            fsm_build_stack(robot, game, PRE_CONSTRUCTION_YELLOW_3, CONSTRUCTION_YELLOW_3);
            if(robot->stack_builded){
                STATE++;
            }
            break;
        
        case 3 : 
            fsm_grab_stack(robot, game, PRE_NODE_STACK_3, NODE_STACK_3);
            if(robot->stack_grabbed){
                STATE++;
            }
            break; 

        case 4 : 
            fsm_build_stack(robot, game, PRE_CONSTRUCTION_YELLOW_2, CONSTRUCTION_YELLOW_2);
            if(robot->stack_builded){
                STATE++;
            }
            break;
        case 5 : 
            fsm_grab_stack(robot, game, PRE_NODE_STACK_7, NODE_STACK_7);
            if(robot->stack_grabbed){
                STATE++;
            }
            break ; 

        case 6 : 
            fsm_build_normal_third_stage(robot, game, PRE_CONSTRUCTION_YELLOW_3, PRE_CONSTRUCTION_YELLOW_2, CONSTRUCTION_YELLOW_2, CONSTRUCTION_YELLOW_3, 37);
            if(robot->stack_builded){
                STATE++;
            }
            break ; 
        case 7 : 
            break ; 
        default : 
            break ; 
    }
}

void start_from_yellow_side(Robot *robot, GAME *game){
    switch (STATE){

        case 1 : // DROP THE BANNER
            drop_banner(robot, game);
            if(robot->banner_dropped){ // WAIT DROP
                 STATE++ ; 
            }
            STATE++;
            break ; 
    }
}

void choose_start(Robot *robot, GAME *game){

    if(get_match_time(game) > 100){
        robot->stop();
        robot->running = false;
        if(match_finished == false){
            fprintf(stderr, "MATCH ENDED by timout\n");
            robot->screen_end_game(); //show the score on the screen
            match_finished = true;
        }
    }

    if(robot->avoidance_loop_activated){
        // DON'T MODIFY THIS LINE
        return; // do nothing is we are avoiding an ennemy
    }

    if((get_match_time(game) > time_return_to_base) && (robot->starting_pos != 2) && (robot->starting_pos != 3)){
        return_to_base(robot, game);
        return;
    }

    switch (robot->starting_pos)
    {
        // 0 = blue_bottom_adversary_bottom
        // 1 = yellow_bottom_adversary_bottom
        // 2 = blue_bottom_adversary_side
        // 3 = yellow_bottom_adversary_side
        // 4 = blue_side
        // 5 = yellow_side

        case 0: // Blue bottom, adversary at bottom
            start_from_blue_bottom_adversary_bottom(robot, game);
            break;
        case 1: // Yellow bottom, adversary at bottom
            start_from_yellow_bottom_adversary_bottom(robot, game);
            break;
        case 2: // Blue bottom, adversary at side
            start_from_blue_bottom_adversary_side(robot, game);
            break;
        case 3: // Yellow bottom, adversary at side
            start_from_yellow_bottom_adversary_side(robot, game);
            break;
        case 4: // Blue side
            start_from_blue_side(robot, game);
            break;
        case 5: // Yellow side
            start_from_yellow_side(robot, game);
            break;
        default:
            fprintf(stderr, "No starting position precised, do it !");
            break;
    }
}

void return_to_base(Robot *robot, GAME *game){
    switch (STATE_RETURN_TO_BASE){
        
        case 0:
            // release the stack
            robot->teensy_send_command(0x30);
            STATE_RETURN_TO_BASE++;
        break;

        case 1:
            // release the stack
            robot->teensy_send_command(0x30);
            STATE_RETURN_TO_BASE++;
            break;

        case 2: // GO TO BASE NODE
            if (robot->starting_pos == 0 || robot->starting_pos == 2 || robot->starting_pos == 4 ){ // in blue team
                PRE_END_ZONE = PRE_END_ZONE_BLUE;
                END_ZONE = END_ZONE_BLUE;
            }
            else if (robot->starting_pos == 1 || robot->starting_pos == 3 || robot->starting_pos == 5 ){ // in yellow team
                PRE_END_ZONE = PRE_END_ZONE_YELLOW;
                END_ZONE = END_ZONE_YELLOW;
            }
            robot->highLevelController(PRE_END_ZONE, game);
            if (robot->end_of_travel){
                STATE_RETURN_TO_BASE++;
            }
            break;

        case 3 : //ORIENTATE TO BASE
            robot->orientate(90, game);
            STATE_RETURN_TO_BASE++;
            break;

        case 4: // ORIENTATING 
            if (robot->end_of_angle){
                STATE_RETURN_TO_BASE++;
            }
            break;

        case 5 : // MANEUVER TO END ZONE   
            if(get_match_time(game) > time_reach_end_zone){
                robot->highLevelController(END_ZONE, game);
                if (robot->end_of_travel){
                    STATE_RETURN_TO_BASE++;
                    robot->points_scored += 10 ;
                }
            }
            break; 

        case 6: // FINISHED MATCH
            fprintf(stderr, "FINISHED MATCH\n");
            //robot->buzzBuzzer();
            STATE_RETURN_TO_BASE++;
            break;

        case 7: // SHUTTING DOWN
            usleep(0.001*1000000);
            break;

    }
}




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