// Robot.cpp
#include "Robot.h"
#include <fstream>

Robot::Robot() 
    : leftMotor(13, 16, 7, 0x12, 0x10, false), // (pwmPin, forwardDirectionPin, backwardDirectionPin, distanceAddress, speedAddress, baseDir)
      rightMotor(12, 23, 27, 0x13, 0x11, true) {
    wiringPiSetup();
}

