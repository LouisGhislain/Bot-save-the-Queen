#include "../../include/Robot.h"

void Robot::teensy_build(void *game){
    int rasp_backward = 0;
    int rasp_forward = 0 ;
    // J'ai déplacé sequence_finished dans Robot.h pour qu'elle soit accessible dans la FSM et je l'ai renommé build_finished
    
    build_finished = false;
    teensy_send_command(0x04);//CMD_BUILD
    usleep(500000);
    maneuver(-0.15,game);//recule avec cremaillère
    usleep(4500000);
    maneuver(0.13,game);//avance avec stack haut
    usleep(4000000);
    maneuver(-0.1,game);
    build_finished = true;

}
/*
    do {
        usleep(10000);  // Attendre 100 ms entre chaque lecture
        rasp_forward = wiringPiI2CRead(fd_teensy);
    } while (rasp_forward != 0x06);
     
*/
