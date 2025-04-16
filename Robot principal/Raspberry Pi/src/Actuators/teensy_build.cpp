#include "../../include/Robot.h"

void Robot::teensy_build(void *game){
    build_finished = false;
    teensy_send_command(0x04);//CMD_BUILD
    usleep(500000);
    straightMotion(-0.15,game);//recule avec cremaillère
    usleep(4500000);
    straightMotion(0.13,game);//avance avec stack haut
    usleep(2000000); //3000000
    straightMotion(-0.1,game);
    build_finished = true;

}
/*
    int rasp_backward = 0;
    int rasp_forward = 0 ;
    do {
        usleep(10000);  // Attendre 100 ms entre chaque lecture
        rasp_forward = wiringPiI2CRead(fd_teensy);
    } while (rasp_forward != 0x06);
     
*/
