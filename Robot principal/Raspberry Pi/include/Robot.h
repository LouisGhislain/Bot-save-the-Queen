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
#include <softTone.h>
#include <stdlib.h>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include <cstdio>
#include <stdio.h>
#include <map>
#include <mutex>
//I2C
#include <wiringPiI2C.h>
#include <string>
#include <cstring>
#include <atomic>
#define SCREEN_ADDR 0x27
#define STARTING_CORD_PIN 20 // starting cord on gpio 20
#define MICROSWITCH_BACK_LEFT 26
#define MICROSWITCH_BACK_RIGHT 21
#define BUZZER_PIN 22  // GPIO22 corresponds to WiringPi pin 3

struct MovementParams {
    bool activated_target_angle;
    double d0;          
    double vMax;        
    double stop_robot_distance; 
    double acceleration;
    double wMax;

    static constexpr double distanceBetweenWheel = 0.25276;

    MovementParams()
        : activated_target_angle(false), d0(0), vMax(0), stop_robot_distance(0), acceleration(0), wMax(0) {}

    MovementParams(bool activated_target_angle, double d0, double vMax, double wMax, double stop_robot_distance)
        : activated_target_angle(activated_target_angle),
          d0(d0),
          vMax(vMax),
          stop_robot_distance(stop_robot_distance),
          acceleration((d0 > 0) ? (vMax * vMax / (2 * d0)) : 0.0),
          wMax(wMax) {}

    bool operator==(const MovementParams& other) const {
        return (activated_target_angle == other.activated_target_angle &&
                d0 == other.d0 &&
                vMax == other.vMax &&
                stop_robot_distance == other.stop_robot_distance &&
                wMax == other.wMax);
        }
};

// Declare as extern to be used in multiple files
extern const MovementParams manoeuvre;
extern const MovementParams deplacement;
extern const MovementParams orientation;
extern const MovementParams straight;

// Declare the function
void controlRobot(const MovementParams& params);

class Robot {
public:
    Robot();
    void start();
    void stop();
    void lowLevelController();
    void middleLevelController(void *game);
    void highLevelController(int goal, void *game);
    void openLoopData();
    void printDistance();
    void lowLevelTest();
    void middleLevelTest(void *game);
    void maneuver(int nodeIndex, void *game);
    void straightMotion(double dist, void *game);
    void orientate(double angle, void *game);

    // Path planning
    void loadNodes(const std::string& filename, void *game);
    void loadEdges(const std::string& filename, void *game);
    void aStar(int start, int goal, void *game);
    double heuristic(const Node& a, const Node& b);
    void printPath();

    // ennemy avoidance
    void stop_if_ennemy();
    std::pair<double, double> get_distance_to_ennemy(GAME *game);
    void reaction_to_ennemy_smart(GAME *game);
    void reaction_to_ennemy_stop(GAME *game);    
    // ennemy avoidance variables
    int CASE_ennemy_avoidance = 0;
    bool avoidance_loop_activated = false; // true if the ennemy avoidance is activated
    std::chrono::steady_clock::time_point starting_ennemy_avoidance_time; // time when the ennemy avoidance started


    // Odometry
    void updateOdometry(void *game);
    void initCoords(void *game);
    int starting_pos;                // 0 = blue_bottom, 1 = blue_side, 2 = yellow_bottom, 3 = yellow_side

    // Buzzer
    void buzzBuzzer();

    // Screen 
    void screen_init();         
    void screen_clear();
    void screen_send_command(int comm);
    void screen_write(int x, int y, const char data[]);
    void screen_write_word(int data);
    void screen_send_data(int data);
    void screen_create_custom_chars();
    void screen_write_custom_char(int x, int y, int custom_char);
    void screen_display_intro();
    void screen_end_game();
    void screen_menu(GAME *game);
    int selected_team ; 
    int opponent_start; 

    // starting cord
    void initialize_pins();
    void wait_starting_cord(GAME *game);
    bool checkIfAgainstWall(GAME *game);
    bool is_right_pressed(GAME *game);
    bool is_left_pressed(GAME *game);
    bool is_stack_present();
    
    // -------Teensy------
    void teensy_init();
    void teensy_send_command(uint8_t command);
    void teensy_build(void *game);
    int teensy_check_IR();
    void teensy_grab();
    void teensy_americain_third_stage(int nodeNumber, void *game);
    void teensy_push_separate(void *game);
    void teensy_separate_third_stage(void *game); //STRAT 1 
    void teensy_build_first_third_stage(int constructNodeNumber, void *game); //STRAT 1 
    void teensy_build_second_third_stage(int constructNodeNumber, void *game); //STRAT 1 
    bool build_finished = false;
    bool separate_finished = false; 

    // Sampling time
    static constexpr double SAMPLING_TIME = 0.001;

    double ref_speed_left = 0.0; // in rad/s (left motor speed)
    double ref_speed_right = 0.0; // in rad/s (right motor speed)
    bool backwards;
    std::mutex ref_speed_mutex;

    double GLOBAL_x_coord_target;
    double GLOBAL_y_coord_target; 
    double GLOBAL_angle_target = M_PI ; // in rad (angle of the robot with respect to the x-axis)
    MovementParams GLOBAL_params = straight;
    double GLOBAL_rho = 0.0; // in m (distance to target)
    std::mutex coord_mutex;
    std::mutex flags;

    // Low level
    double u_volt_left = 0;
    double u_volt_right = 0;

    // Middle level
    bool end_of_manoeuvre = false; // true if the robot has reached the target position
    bool end_of_angle = false; // true if the robot has reached the target angle

    // High level
    bool end_of_travel = true;
    double rho_to_goal;
    std::mutex rho_to_goal_mutex;

    // FSM
    bool arrived_first_stack = false;
    bool grab_command_sent = false;
    bool stack_builded = false ; 
    bool banner_dropped = false ; 
    bool stack_grabbed = false ;

    //Number of points (count for screen)
    int points_scored = 0 ;

    // MAIN
    std::atomic<bool> running{false}; // Flag to control the main loop

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

    // Robot parameters
	static constexpr double distanceBetweenWheels = 0.25276; // in m

    // Back EMF Constant
    static constexpr double K_phi = 0.02859; // in V/(rad/s) (Back EMF constant for the motors)

    // Low level controller variables
    // static constexpr double KpPos = 0.01;
    // static constexpr double KiPos = 0.0;
    static constexpr double KpSpeed = 1.4048464035277164;
    static constexpr double KiSpeed = 2.6222100340415317;

    // Middle level controller variable
    static constexpr double KpAlpha = 6;
    static constexpr double Kp_orientation = 7; // in rad/s/rad (proportional gain for the orientation controller)    

    double delta_x_target;
    double delta_y_target;
    double last_distl_high = 0;
    double last_distr_high = 0;
    double travelled_distance = 0.0; // in m (distance from the starting point)
    std::mutex travelled_distance_mutex;
    double v_ref = 0.0; // in m/s (linear speed)
    double v_threshold_move = 0.0441; // in m/s (minimum speed to move) (1.5 rad/s a la roue) = 1.5 * wheel_radius = 0.0441 m/s

    // High level controller variables
    int current_destination = 1914; // la guerre est déclarée
    int current_step = 0;
    const double d1_change_target = 0.04; // in m (distance from when we follow the next node on the path)
    bool last_step = true;

    // Path planning variables
    std::vector<int> path; // Path to follow

    // SPI Constants
    static constexpr int SPI_CHANNEL = 0;
    static constexpr int SPI_SPEED = 7500000; // 7.5 MHz
 
    // Odometry variables
    double last_distl = 0.0;             // in m
    double last_distr = 0.0;
    double distl = 0.0;
    double distr = 0.0;
    std::mutex dist_mutex;
    double distanceBetweenOdometers = 0.28806; // in m (distance between the two wheels)
    double wheel_radius = 0.0295;              // in m (radius of the wheels)

    double starting_angle = 0.0;         // in radians (initial angle of the robot, 0 = x-axis)

    //State pin teensy (not use anymore)
    static constexpr int STATE0_PIN = 35;
    static constexpr int STATE1_PIN = 31;
    static constexpr int STATE2_PIN = 29;
    
    // File descriptor for I2C
    int fd_screen;
    int fd_teensy;      
};

#endif // ROBOT_H

