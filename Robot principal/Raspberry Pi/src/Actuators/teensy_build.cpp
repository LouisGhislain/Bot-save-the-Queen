#include "../../include/Robot.h"

void Robot::teensy_build(void *game){
    int rasp_backward = 0;
    int rasp_forward = 0 ;
    int sequence_finished = 0 ;

    
    teensy_send_command(0x04);//CMD_BUILD
    maneuver(-0.15,game);
    usleep(5500000);
    maneuver(0.15,game);
    usleep(4000000);
    maneuver(-0.1,game);

    
}
/*
    do {
        usleep(10000);  // Attendre 100 ms entre chaque lecture
        rasp_forward = wiringPiI2CRead(fd_teensy);
    } while (rasp_forward != 0x06);
     
*/
