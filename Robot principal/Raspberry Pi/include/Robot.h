// Robot.h
#ifndef ROBOT_H
#define ROBOT_H

#include "Motor.h"
#include <wiringPi.h>
#include "struct.h"
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

struct MovementParams {
    bool activated_target_angle;
    double d0;          
    double vMax;        
    double stop_robot_distance; 
    double acceleration;
    double wmax;

    static constexpr double distanceBetweenWheel = 0.25276;

    MovementParams()
        : activated_target_angle(false), d0(0), vMax(0), stop_robot_distance(0), acceleration(0), wmax(0) {}

    MovementParams(bool activated_target_angle, double d0, double vMax, double stop_robot_distance)
        : activated_target_angle(activated_target_angle),
          d0(d0),
          vMax(vMax),
          stop_robot_distance(stop_robot_distance),
          acceleration((d0 > 0) ? (vMax * vMax / (2 * d0)) : 0.0),
          wmax((distanceBetweenWheel > 0) ? (2 * vMax / distanceBetweenWheel) : 0.0) {}
};

// Declare as extern to be used in multiple files
extern const MovementParams manoeuvre;
extern const MovementParams deplacement;
extern const MovementParams orientation;

// Declare the function
void controlRobot(const MovementParams& params);

class Robot {
public:
    Robot();
    void start();
    void stop();
    void lowLevelController(double ref_speed_left, double ref_speed_right);
    void middleLevelController(double x_coord_target, double y_coord_target, double goal_angle, const MovementParams& params, void *game);
    void highLevelController(void *game);
    void openLoopData();
    void printDistance();
    void lowLevelTest();
    void buzzBuzzer();
    void updateOdometry(void *game);
    void initCoords(void *game);
    void screen_init();         
    void screen_clear();        
    void screen_displayText(const std::string &text); 
    void teensy_init();
    void teensy_send_command(uint8_t command);
    void teensy_build();
    int teensy_check_IR();
    void teensy_grab();
    void lowLevelForward();
    void lowLevelBackward();
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
    static constexpr double KpSpeed = 1.4048464035277164;
    static constexpr double KiSpeed = 2.6222100340415317;

    // Middle level controller gains
    static constexpr double KpAlpha = 5.0;
    static constexpr double KpBeta = -4.0;

    // Middle level controller variables
    double delta_x_target;
    double delta_y_target;

    // High level controller variables (used in middle)
    double rho = 0.0; // in m (distance to target)
    double travelled_distance = 0.0; // in m (distance from the starting point)
    double v_ref = 0.0; // in m/s (linear speed)

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
    
    // File descriptor for I2C
    int fd_OLED;
    int fd_teensy;
        
    
};

#endif // ROBOT_H

