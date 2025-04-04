// Robot.h
#pragma once

#include "Motor.h"
#include <wiringPi.h>
#include "SharedStruct.h"
#include <cstdint>
#include <cmath>
#include <unistd.h> // For sleep function
#include <iostream>
#include <vector>
#include <tuple>

// Use a common base struct or class for polymorphism if needed
struct MovementParams {
    bool activated_target_angle;
    double d0;          // in m (distance of acceleration phase)
    double vMax;         // in m/s (maximum speed)
    double acceleration; // in m/s²
    double stop_robot_distance; // in m
    double wmax; // in rad/s (maximum angular speed)
    
    // Constructor to ensure proper initialization
    MovementParams(bool angle, double d, double v, double stop_dist)
        : activated_target_angle(angle), d0(d), vMax(v),
          acceleration((v * v) / (2 * d)), stop_robot_distance(stop_dist), wmax(2 * v / distanceBetweenWheels) {}
};

// Then define your specific configurations
const MovementParams manoeuvre {
    true,   // activated_target_angle
    0.005,  // d0
    0.2,    // vMax
    0.001   // stop_robot_distance
};

const MovementParams deplacement {
    false,  // activated_target_angle
    0.010,  // d0
    0.5,    // vMax
    0.005   // stop_robot_distance
};

const MovementParams orientation {
    true,   // activated_target_angle
    0.005,  // d0
    0.0,    // vMax
    0.001   // stop_robot_distance
};

// Your function would look like:
void controlRobot(const MovementParams& params) {
    // Use params.activated_target_angle, params.vMax, etc.
}
class Robot {
public:
    Robot();
    void start();
    void stop();
    void lowLevelController(double ref_speed_left, double ref_speed_right);
    void middleLevelController(double x, double y, double goal_angle);
    void openLoopData();
    void printDistance();
    void lowLevelTest();
    void buzzBuzzer();
    void teensy_cans();
    void teensy_lift();
    void teensy_cans_lift();
    void updateOdometry();
    void initCoords();
    //void teensy_push();

    // Sampling time
    static constexpr double SAMPLING_TIME = 0.001;

private:
    void initializeSPI(); 
    void resetValues();

    Motor leftMotor;
    Motor rightMotor;

    // Motor control variables
    double intESpeedLeft = 0.0;
    double intESpeedRight = 0.0;
    double intEPosLeft = 0.0;
    double intEPosRight = 0.0;
    double u_volt_left = 0;
    double u_volt_right = 0;

    // Robot parameters
	static constexpr double distanceBetweenWheels = 0.25276; // in m

    // Back EMF Constant
    static constexpr double K_phi = 0.02859; // in V/(rad/s) (Back EMF constant for the motors)

    // PI gains
    static constexpr double KpPos = 0.01;
    static constexpr double KiPos = 0.0;
    static constexpr double KpSpeed = 1.1218459631554034;
    static constexpr double KiSpeed = 2.8040330710262706;

    // Middle level controller gains
    static constexpr double KpAlpha = 5.0;
    static constexpr double KpBeta = -4.0;

    // Middle level controller variables
    double distl = 0.0; // in m
    double distr = 0.0; // in m
    double last_distl = 0.0; // in m
    double last_distr = 0.0; // in m
    double delta_x_target;
    double delta_y_target;

    // High level controller variables (used in middle)
    double rho = 0.0; // in m (distance to target)
    double travelled_distance = 0.0; // in m (distance from the starting point)
    double vref = 0.0; // in m/s (linear speed)

    // SPI Constants
    static constexpr int SPI_CHANNEL = 0;
    static constexpr int SPI_SPEED = 7500000; // 7.5 MHz
 
    // Odometry variables
    double last_distl = 0.0;             // in m
    double last_distr = 0.0;
    double distl = 0.0;
    double distr = 0.0;
    double distanceBetweenOdometers = 0.28806; // in m (distance between the two wheels)
    double wheel_radius = 0.0295;         // in m (radius of the wheels)

    int starting_pos = 0;                // 0 = blue_bottom, 1 = blue_side, 2 = yellow_bottom, 3 = yellow_side
    double starting_angle = 0.0;         // in radians (initial angle of the robot, 0 = x-axis)

    //State pin teensy
    static constexpr int STATE0_PIN = 35;
    static constexpr int STATE1_PIN = 31;
    static constexpr int STATE2_PIN = 29;
    
};

