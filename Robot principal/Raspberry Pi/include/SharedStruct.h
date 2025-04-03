//==============================================================
// not yet ready
//===============================================================

#pragma once

typedef struct shared_variables {
    // Odometry variables
    double xCoord = 0.0;                 // in m (x-coordinate of the robot)
    double yCoord = 0.0;                 // in m (y-coordinate of the robot)
    double theta = 0.0;                  // in radians (angle of the robot, 0 = x-axis)
} shared_variables;

extern shared_variables sv;