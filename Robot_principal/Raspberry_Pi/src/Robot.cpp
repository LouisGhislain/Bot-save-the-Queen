// Robot.cpp
#include "Robot.h"
#include <fstream>

Robot::Robot() 
    : leftMotor(13, 16, 7, 0x12, 0x10, false, 0.0445*0.99967), // (pwmPin, forwardDirectionPin, backwardDirectionPin, distanceAddress, speedAddress, baseDir, odometerDiameter)
      rightMotor(12, 23, 27, 0x13, 0x11, true, 0.0445*0.99773) { //
    wiringPiSetup();
}

