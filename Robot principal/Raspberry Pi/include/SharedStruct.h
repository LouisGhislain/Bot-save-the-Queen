//==============================================================
// not yet ready
//===============================================================

#pragma once

typedef struct shared_variables {
    // Odometry variables
    double xCoord = 0.0;                 // in m (x-coordinate of the robot)
    double yCoord = 0.0;                 // in m (y-coordinate of the robot)
    double theta = 0.0;                  // in radians (angle of the robot, 0 = x-axis)

    // Motor control variables
    double intESpeedLeft = 0.0;
    double intESpeedRight = 0.0;
    double intEPosLeft = 0.0;
    double intEPosRight = 0.0;
    double u_volt_left = 0;
    double u_volt_right = 0;

    // Displacement tracking
    double lastLeftDistance = 0.0;
    double lastRightDistance = 0.0;
    
    // Odometry variables
    double last_distl = 0.0;             // in m
    double last_distr = 0.0;
    double distl = 0.0;
    double distr = 0.0;
    
    // Constants (could also be moved here if needed by other components)
    // static constexpr double distanceBetweenOdometers = 0.28806; // in m
    // static constexpr double wheel_radius = 0.0295;         // in m
} shared_variables;