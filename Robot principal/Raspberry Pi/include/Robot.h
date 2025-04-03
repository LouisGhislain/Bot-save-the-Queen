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



    // Back EMF Constant
    static constexpr double K_phi = 0.02859; // in V/(rad/s) (Back EMF constant for the motors)

    // PI gains
    static constexpr double KpPos = 0.01;
    static constexpr double KiPos = 0.0;
    static constexpr double KpSpeed = 1.4048464035277164;
    static constexpr double KiSpeed = 2.6222100340415317;

    // Middle level controller gains
    static constexpr double KpRho = 0.00333;
    static constexpr double KpAlpha = 0.004;
    static constexpr double KpBeta = 0.0;

    // Displacement tracking
    double lastLeftDistance = 0.0;
    double lastRightDistance = 0.0;

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

