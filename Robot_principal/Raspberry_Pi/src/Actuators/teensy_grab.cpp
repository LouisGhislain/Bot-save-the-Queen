#include "../../include/Robot.h"

void Robot::teensy_grab(){
    teensy_send_command(0x42); //CMD grab
    usleep(1.2*1000000); //before 1
}