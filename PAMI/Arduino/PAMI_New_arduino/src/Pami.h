#pragma once 
#include "Motor.h"
#include "Encoder.h"
#include "Tail.h"
#include "Microswitch.h"
#include "Sonar.h"


typedef enum {
    waiting_start,
    moving,
    dancing
} State_t;

class PAMI {
public:
    PAMI();
    void getAngle();
    void lowlevelcontrol(double ref_speed_left, double ref_speed_right);
    void pami_brake();

    State_t getState();
    void setState(State_t new_state);
    bool isStartPressed();
    bool isLeftPressed();
    double getSonarDistance();
    void turnTail();


private:
    Encoder leftEncoder;
    Encoder rightEncoder;
    Motor leftMotor;
    Motor rightMotor;
    Microswitch starting_switch;
    Microswitch left_switch;
    Microswitch right_switch;
    Tail tail;
    Sonar sonar;
    State_t PAMI_state ; 

    float last_time_ctrl = 0;
    float current_time_ctrl = 0;

    double left_voltage = 0;
    double right_voltage = 0;

    double left_speed = 0;
    double right_speed = 0;

    double left_position = 0;
    double right_position = 0;


    static constexpr float Kp_left = 30; // Coefficient proportionnel pour le moteur gauche
    static constexpr float Ki_left = 300; // Coefficient intégral pour le moteur gauche
    static constexpr float Kp_right = 30; // Coefficient proportionnel pour le moteur droit
    static constexpr float Ki_right = 300; // Coefficient intégral pour le moteur droit

    
};