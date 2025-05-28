#include "../../../include/Robot.h"
#include <iostream>

void Robot::printDistance(){
    std::cout<< "Left distance: "<< leftMotor.getDistance() << "     Right distance: "<< rightMotor.getDistance() << std::endl;
}
