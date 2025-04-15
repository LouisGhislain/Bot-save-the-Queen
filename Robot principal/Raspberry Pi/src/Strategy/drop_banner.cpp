#include "../../include/FSM.h"
int BANNER_STATE = 0 ;

void drop_banner(Robot *robot, GAME *game){
    // Print BANNER_STATE
    std::cout << "BANNER_STATE: " << BANNER_STATE << std::endl;
    robot->banner_dropped = false ;
    switch(BANNER_STATE){
    case 0 : //backward to drop 
        //robot->straightMotion(-0.10, game);
        BANNER_STATE++;
        break;
    case 1 :
        // Print check if against wall
        std::cout << "check if against wall: " << robot->checkIfAgainstWall(game) << std::endl;
        if (robot->checkIfAgainstWall(game)){
            //usleep(100000);
            BANNER_STATE++;
        }
        break;
    case 2 : 
        robot->straightMotion(0.045,game);
        BANNER_STATE++;
        break ; 
    case 3 : 
        if(robot->end_of_manoeuvre){
            robot->banner_dropped = true ; 
            BANNER_STATE = 0 ;
        }
    }
}