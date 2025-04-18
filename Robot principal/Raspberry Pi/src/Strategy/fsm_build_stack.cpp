#include "../../include/FSM.h"
int STATE_BUILDING = 0;

void fsm_build_stack(Robot *robot, GAME *game, int PRE_NODE, int NODE){
    robot->stack_builded = false ;
    GAME * mygame = (GAME *)game;
    MAP * mymap = mygame->map;
    Queen * myqueen = mygame->queen;

    switch(STATE_BUILDING){

        case 0 : //PATH PLANNING TO PRE_NODE BUILDING ZONE
            if ((NODE == CONSTRUCTION_YELLOW_0)||(NODE ==  CONSTRUCTION_BLUE_0)){
                double dist_to_prenode;
                {
                    std::lock_guard<std::mutex> lock(myqueen->position_mutex);
                    dist_to_prenode = sqrt(pow((mymap->nodes[PRE_NODE].x - myqueen->cart_pos->x),2) + pow((mymap->nodes[PRE_NODE].y - myqueen->cart_pos->y),2));
                }
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
            double dist_a_avancer;
            if ((NODE == CONSTRUCTION_YELLOW_0)||(NODE ==  CONSTRUCTION_BLUE_0)){
                dist_a_avancer = 0.09;
            }
            else{
                dist_a_avancer = DISTANCE_NODE_PRE_NODE;
            }
            robot->straightMotion(dist_a_avancer, game); //robot->maneuver(NODE, game);
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
                robot->points_scored += 12 ; //4+8
            }
            break;
    }
}


void fsm_build_american_stage(Robot *robot, GAME *game, int PRE_NODE, int NODE){
    robot->stack_builded = false ; 
    switch(STATE_BUILDING){

        case 0 : // MOVING TO STACK
            robot->highLevelController(PRE_NODE, game);
            if (robot->end_of_travel){
                STATE_BUILDING++;
            }
            break ; 

        case 1 : // ORIENTATE TO BUILD ZONE
            robot->orientate(-90, game);
            STATE_BUILDING++;
            break;

        case 2: // ORIENTATING   
            if (robot->end_of_angle){
                STATE_BUILDING++;
            }
            break;

        case 3 : // BUILD AMERICAIN STAGE
            robot->teensy_americain_third_stage(NODE, game);
            STATE_BUILDING++;
            break ; 

        case 4 : // BUILDING
            if (robot->build_finished){
                STATE_BUILDING++;
            }
            break ; 

        case 5 : // BACKWARD MANEUVER AFTER BUILDING
            robot->maneuver(PRE_NODE, game);            
            STATE_BUILDING++;
            break;

        case 6 : // MANEUVRING
            if (robot->end_of_manoeuvre){
                robot->stack_builded = true ; 
                STATE_BUILDING=0;
                robot->points_scored+=16;
            }
            break;

    }
}
    
void fsm_build_normal_third_stage(Robot *robot, GAME *game, int SEPARATE_NODE, int PRE_CONSTRUCT_NODE_1, int CONSTRUCT_NODE_1, int CONSTRUCT_NODE_2, int PRE_PRE_CONSTRUCT_NODE_1){
    robot->stack_builded = false ;
    std::cout << "State building : " << STATE_BUILDING << std::endl;
    switch(STATE_BUILDING){
        
        case 0 : // MOVING TO PRE NODE TO SEPARATE
            robot->highLevelController(SEPARATE_NODE, game);
            if (robot->end_of_travel){
                STATE_BUILDING++;
            }
            break ; 

        case 1 : // ORIENTATE TO SEPARATE ZONE
            robot->orientate(-90, game);
            STATE_BUILDING++;
            break;

        case 2: // ORIENTATING
            if (robot->end_of_angle){
                STATE_BUILDING++;
            }
            break;

        case 3 : // SEPARATE 
            robot->teensy_separate_third_stage(game);
            robot->points_scored+=4 ; 
            STATE_BUILDING++;
            break ; 

        case 4 : // BACKWARD A LOT
            if(robot->separate_finished){
            //if(robot->separate_finished && robot->end_of_manoeuvre){
                robot->straightMotion(-0.20, game); //CHANGER EN MANEUVER ?
                STATE_BUILDING++;
            }
            break ; 

        case 5 : // MANEUVERING
            if(robot->end_of_manoeuvre){
                usleep(1000000); //enlever si on a changé la vitesse lift
                STATE_BUILDING++;
            }
            break ; 

        case 6 : // MOVING TO PRE NODE CONSTRUCT 1
            robot->highLevelController(PRE_CONSTRUCT_NODE_1, game);
            if (robot->end_of_travel){
                STATE_BUILDING++;
            }
            break ; 

        case 7: 
            robot->teensy_build_first_third_stage(CONSTRUCT_NODE_1,game);
            STATE_BUILDING++;
            break ; 

        case 8 : // BACKWARD A LOT
            if(robot->build_finished){
               robot->straightMotion(-0.25, game); //CHANGER EN MANEUVER ?
               robot->points_scored+=16;
                STATE_BUILDING++;
            }
            break ; 

        case 9 : // MANEUVRING
            if(robot->end_of_manoeuvre){
                STATE_BUILDING++;
            }
            break ; 

        case 10 : // MOVING TO NODE BEFORE SEPARATE STACK
            robot->highLevelController(PRE_PRE_CONSTRUCT_NODE_1, game);
            if (robot->end_of_travel){
                STATE_BUILDING++;
            }
            break ; 

        case 11 : //ORIENTATE TO BASE
            robot->orientate(-90, game);
            STATE_BUILDING++;
            break;

        case 12: // ORIENTATING 
            if (robot->end_of_angle){
                STATE_BUILDING++;
            }
            break;

        case 13 : //MANEUVER TO SEPARATE STACK
            robot->maneuver(SEPARATE_NODE, game);
            STATE_BUILDING++;
            break; 

        case 14 : //MANEUVRING
            if(robot->end_of_manoeuvre){
                robot->straightMotion(0.05, game);
                STATE_BUILDING++;
            }
            break ; 
        
        case 15 : 
            if(robot->end_of_manoeuvre){
                STATE_BUILDING++;
            }
            break ; 
            
        case 16 : 
            //usleep(1000000);
            robot->teensy_build_second_third_stage(CONSTRUCT_NODE_2, game);
            STATE_BUILDING++;
            break ; 
        
        case 17 : 
            if(robot->build_finished){
                robot->straightMotion(-0.25, game); //CHANGER EN MANEUVER ?
                STATE_BUILDING++;
            }
            break ; 

        case 18 : 
            if(robot->end_of_manoeuvre){
                robot->stack_builded = true ; 
                STATE_BUILDING = 0 ; 
                robot->points_scored += 12 ; //16 - 4 (on a enlevé un de 1 étage)
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
                robot->points_scored += 30 ; 

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
    if(robot->end_of_manoeuver){
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