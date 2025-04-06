#pragma once 
#include "Motor.h"
#include "Encoder.h"
#include "Tail.h"
#include "Microswitch.h"
#include "Sonar.h"

class PAMI {
public:
    PAMI();
    void getAngle();
    void lowlevelcontrol(double ref_speed_left, double ref_speed_right);


private:
    Encoder leftEncoder;
    Encoder rightEncoder;
    Motor leftMotor;
    Motor rightMotor;
    // Tail tail;
    // Microswitch microswitch;
    // Sonar sonar;
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