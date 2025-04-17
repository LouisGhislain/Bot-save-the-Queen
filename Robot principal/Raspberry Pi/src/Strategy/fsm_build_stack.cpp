#include "../../include/FSM.h"
int STATE_BUILDING = 0;

void fsm_build_stack(Robot *robot, GAME *game, int PRE_NODE, int NODE){
    robot->stack_builded = false ;
    GAME * mygame = (GAME *)game;
    MAP * mymap = mygame->map;
    Queen * myqueen = mygame->queen;

    switch(STATE_BUILDING){

        case 0 : //PATH PLANNING TO PRE_NODE BUILDING ZONE
            if ((NODE == CONSTRUCTION_YELLOW_0)||(NODE ==  CONSTRUCTION_YELLOW_2)){
                double dist_to_prenode;
                {
                    std::lock_guard<std::mutex> lock(myqueen->position_mutex);
                    dist_to_prenode = sqrt(pow((mymap->nodes[PRE_NODE].x - myqueen->cart_pos->x),2) + pow((mymap->nodes[PRE_NODE].y - myqueen->cart_pos->y),2));
                }
                // Print dist_to_prenode
                std::cout << "dist_to_prenode: " << dist_to_prenode << std::endl;
                if (dist_to_prenode < 0.15){
                    STATE_BUILDING++;
                    break;
                }
            }
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
    
void fsm_build_normal_third_stage(Robot *robot, GAME *game, int SEPARATE_NODE, int PRE_CONSTRUCT_NODE_1, int CONSTRUCT_NODE_1, int CONSTRUCT_NODE_2){
    robot->stack_builded = false ;
    std::cout << "Robot_third_stage state: " << STATE_BUILDING << std::endl;
    switch(STATE_BUILDING){
        
        case 0 : //MOVING TO PRE NODE TO SEPARATE
            robot->highLevelController(SEPARATE_NODE, game);
            if (robot->end_of_travel){
                STATE_BUILDING++;
            }
            break ; 

        case 1 : //ORIENTING 
            robot->orientate(-90, game);
            if (robot->end_of_angle){
                STATE_BUILDING++;
            }
            break;

        case 2 : //SEPARATE 
            robot->teensy_separate_third_stage(game);
            STATE_BUILDING++;
            break ; 

        case 3 : //BACKWARD A LOT
            if(robot->separate_finished && robot->end_of_manoeuvre){
                robot->straightMotion(-0.15, game); //CHANGER EN MANEUVER ?
                std::cout << "Target y : " << robot->GLOBAL_y_coord_target << std::endl;
                STATE_BUILDING++;
            }
            break ; 

        case 4 : //MANEUVRING
            if(robot->end_of_manoeuvre){
                usleep(1000000); //enlever si on a changé la vitesse lift
                STATE_BUILDING++;
            }
            break ; 

        case 5 : //MOVING TO PRE NODE CONSTRUCT 1
            robot->highLevelController(PRE_CONSTRUCT_NODE_1, game);
            if (robot->end_of_travel){
                STATE_BUILDING++;
            }
            break ; 

        case 6 : 
            //usleep(2000000);
            robot->teensy_build_first_third_stage(CONSTRUCT_NODE_1,game);
            STATE_BUILDING++;
            break ; 

        case 7 : //BACKWARD A LOT
            if(robot->build_finished){
                robot->straightMotion(-0.25, game); //CHANGER EN MANEUVER ?
                STATE_BUILDING++;
            }
            break ; 

        case 8 : //MANEUVRING
            if(robot->end_of_manoeuvre){
                STATE_BUILDING++;
            }
            break ; 

        case 9 : //MOVING TO SEPARATE STACK
            robot->highLevelController(SEPARATE_NODE, game);
            if (robot->end_of_travel){
                STATE_BUILDING++;
            }
            break ; 
        case 10 : 
            robot->straightMotion(0.05,game);
            STATE++;
        case 11 : 
            if(robot->end_of_manoeuvre){
                robot->teensy_build_second_third_stage(CONSTRUCT_NODE_2, game);
                STATE_BUILDING++;
            }
            break ; 

        case 12 : 
            if(robot->build_finished){
                robot->stack_builded = true ; 
                STATE_BUILDING = 0 ; 
            }
            break ;
        
    }
}



void fsm_build_romain(Robot *robot, GAME *game, int PRE_NODE, int GRAB_NODE){
    robot->stack_builded = false ;
    switch(STATE_BUILDING){
        case 0 :
            robot->highLevelController(PRE_NODE, game);
            if (robot->end_of_travel){
                STATE_BUILDING++;
            }
            break ; 
        case 1:
            robot->straightMotion(0.21 + 0.0, game);
            STATE_BUILDING++;
            break;
        case 2 : 
            if (robot->end_of_manoeuvre){
                robot->teensy_push_separate(game);
                STATE_BUILDING++;
            }
            break;
        case 3 : 
            if(robot->separate_finished){
                STATE_BUILDING=0;
                robot->stack_builded = true ;
            }
        break ; 
    }
}

/*
case 3 : 
    if(robot->separated_finished){
        //RECULE
        STATE_BUILDING++;
    }
    break ; 
case 4 : //MANEUVRING
    if(robot->end_of_maneuver){
        STATE_BUILDING++;
    }
    break ; 
case 5 : //Move to construct
    robot->highLevelController(PRE_NODE_CONSTRUCT, game);
    if (robot->end_of_travel){
        STATE_BUILDING++;
    }
    break ; 
 case 6 : //ORIENTING TO BUILD ZONE
    robot->orientate(-90, game);
    if (robot->end_of_angle){
        STATE_BUILDING++;
    }
    break;
case 6 : //UP AND RELEASE
    robot->
    */

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