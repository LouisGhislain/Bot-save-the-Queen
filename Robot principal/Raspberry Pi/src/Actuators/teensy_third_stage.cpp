#include "../../include/Robot.h"

void Robot::teensy_americain_third_stage(int nodeNumber, void *game){
    build_finished = false;
    teensy_send_command(0x12); //CMD_AMERICAIN_THIRD_STAGE
    usleep(2000000);
    maneuver(nodeNumber, game); //forward
    usleep(4000000);
    straightMotion(-0.15,game); //backward down lift
    build_finished = true ; 
}