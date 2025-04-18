#include "../../include/Robot.h"

int Robot::teensy_check_IR(){
    int position;
    for (int i = 0; i < 100; i++)
    {
        usleep(100000); // Pause de 100ms
        position = wiringPiI2CReadReg16(fd_teensy,0x01);
        std::cout << "Position : " << position << " cm" << std::endl;
    }
    return position ; 
}