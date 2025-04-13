#include "../../include/Robot.h"

void Robot::teensy_grab(){
    teensy_send_command(0x02); //CMD grab
    usleep(2000000);
}