// Robot.h
#pragma once

#include "Motor.h"
#include <wiringPi.h>
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
    double getAngle() const;
    void lowLevelController(double ref_speed_left, double ref_speed_right);
    void middleLevelController(double x, double y, double goal_angle);
    void routine();
    void openLoopData();
    void printDistance();
    void lowLevelTest();
    void buzzBuzzer();
    void teensy_cans();
    void teensy_lift();
    void teensy_cans_lift();
    //void teensy_push();

private:
    void initializeSPI(); 
    void resetValues();

    Motor leftMotor;
    Motor rightMotor;

    double intESpeedLeft = 0.0;
    double intESpeedRight = 0.0;
    double intEPosLeft = 0.0;
    double intEPosRight = 0.0;
    double u_volt_left = 0;
    double u_volt_right = 0;

    // Sampling time
    static constexpr double SAMPLING_TIME = 0.01;

    // PI gains
    static constexpr double KpPos = 0.01;
    static constexpr double KiPos = 0.0;
    static constexpr double KpSpeed = 0.1825431783524443; //0.27381476752866646; //0.5476295350573329;
    static constexpr double KiSpeed = 0.48037678513801135; //0.7205651777070171; //1.4411303554140342;

    // Robot position parameters
    double xCoord = 3.8;
    double yCoord = 12.5;

    // Middle level controller gains
    static constexpr double KpRho = 0.00333;
    static constexpr double KpAlpha = 0.004;
    static constexpr double KpBeta = 0.0;

    // Displacement tracking
    double lastLeftDistance = 0.0;
    double lastRightDistance = 0.0;

    // SPI Constants
    static constexpr int SPI_CHANNEL = 0;
    static constexpr int SPI_SPEED = 50000;
    
    //State pin teensy
    static constexpr int STATE0_PIN = 35;
    static constexpr int STATE1_PIN = 31;
    static constexpr int STATE2_PIN = 29;
    
    
};

