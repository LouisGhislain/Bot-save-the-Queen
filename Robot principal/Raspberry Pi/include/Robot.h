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
//I2C
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <string>
#define OLED_ADDR 0x3C 
#define OLED_CMD  0x00  
#define OLED_DATA 0x40  

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
    void updateOdometry();
    void initCoords();
    void screen_init();         
    void screen_clear();        
    void screen_displayText(const std::string &text); 
    void teensy_init();
    void teensy_send_command(uint8_t command);
    void teensy_separate_stack();
    int teensy_check_IR();

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
    static constexpr double SAMPLING_TIME = 0.001;

    // PI gains
    static constexpr double KpPos = 0.01;
    static constexpr double KiPos = 0.0;
    static constexpr double KpSpeed = 0.1825431783524443; //0.27381476752866646; //0.5476295350573329;
    static constexpr double KiSpeed = 0.48037678513801135; //0.7205651777070171; //1.4411303554140342;

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

    // Odometry variables
    double last_distl = 0.0;             // in m
    double last_distr = 0.0;
    double distl = 0.0;
    double distr = 0.0;
    double distanceBetweenWheels = 0.15; // in m (distance between the two wheels)
    double wheel_radius = 0.035;         // in m (radius of the wheels)

    double xCoord = 0.0;                 // in m (x-coordinate of the robot)
    double yCoord = 0.0;                 // in m (y-coordinate of the robot)
    double theta = 0.0;                  // in radians (angle of the robot, 0 = x-axis)

    int starting_pos = 0;                // 0 = blue_bottom, 1 = blue_side, 2 = yellow_bottom, 3 = yellow_side
    double starting_angle = 0.0;         // in radians (initial angle of the robot, 0 = x-axis)

    //State pin teensy
    static constexpr int STATE0_PIN = 35;
    static constexpr int STATE1_PIN = 31;
    static constexpr int STATE2_PIN = 29;
    
    // File descriptor for I2C
    int fd_OLED;
    int fd_teensy;
        
    
};

