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

void Robot::teensy_separate_third_stage(void *game){
    separate_finished = false ; 
    teensy_send_command(0x13);
    usleep(500000); //delay test build
    straightMotion(-0.15, game); //recule avec cremaillère
    usleep(300000); //PAS FIXE mais time de reculer les cremaillères
    separate_finished = true ; 
}

void Robot::teensy_build_first_third_stage(int constructNodeNumber, void *game){
    build_finished = false ; 
    teensy_send_command(0x14);
    maneuver(constructNodeNumber, game);//forward
    usleep(4000000);
    straightMotion(-0.15, game); //backward down lift
    build_finished = true ; 
}

void Robot::teensy_build_second_third_stage(int constructNodeNumber, void *game){
    build_finished = false ; 
    teensy_send_command(0x15); 
    usleep(2000000);
    maneuver(constructNodeNumber, game); //forward
    usleep(4000000);
    straightMotion(-0.15,game); //backward down lift
    build_finished = true ; 
}