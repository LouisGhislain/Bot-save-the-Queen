#include "../../include/Robot.h"

void Robot::teensy_americain_third_stage(void *game){
    build_finished = false;
    teensy_send_command(0x12); //CMD_AMERICAIN_THIRD_STAGE
    usleep(2000000);
    maneuver(0.15,game); //forward to pose the stack
    usleep(4000000);
    maneuver(-0.15,game); //backward down lift
    build_finished = true ; 
}