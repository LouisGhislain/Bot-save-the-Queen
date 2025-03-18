#include "../../include/Robot.h"
#include <iostream>

void Robot::printDistance(){
    while(true){
    std::cout<< "Left distance: "<< leftMotor.getDistance() << "     Right distance: "<< rightMotor.getDistance() << std::endl;
    }
}
