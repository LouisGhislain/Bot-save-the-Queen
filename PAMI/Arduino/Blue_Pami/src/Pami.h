#pragma once 
#include "Motor.h"
#include "Encoder.h"
#include "Tail.h"
#include "Sonar.h"


typedef enum {
    waiting_start,
    moving,
    dancing
} State_t;

class PAMI {
public:
    PAMI();
    void lowlevelcontrol(double ref_speed_left, double ref_speed_right);
    double getX_position();
    double getY_position();
    void pami_brake();
    void middlecontrol(double x_ref, double y_ref, double angle_ref, bool target);
    void update_position();
    double getAngle();
    void stop();
    bool target_reached = false; // Indicateur de cible atteinte
    bool start_angle = false; // Indicateur de début d'angle    void pami_brake();

    State_t getState();
    void setState(State_t new_state);
    bool isStartPressed();
    bool isLeftPressed();
    double getSonarDistance();
    void turnTail();
    void middlecontrol_switch(double x_ref, double y_ref, double angle_ref, bool target);
    void Turn(double angle_ref);
    void reset_last_time_ctrl();
    void reset_position();
    void Rotate(double angle_ref);

private:
    Encoder leftEncoder;
    Encoder rightEncoder;
    Motor leftMotor;
    Motor rightMotor;

    Tail tail;
    Sonar sonar;
    State_t PAMI_state ; 

    double last_time_ctrl = 0;
    double current_time_ctrl = 0;

    double left_voltage = 0;
    double right_voltage = 0;

    double left_speed = 0;
    double right_speed = 0;

    double left_position = 0;
    double right_position = 0;

    double x_position = 0;
    double y_position = 0;
    double angle = 0;

    double x_start = 0;
    double y_start = 0;

    double dist_left = 0;
    double dist_right = 0;

    double last_dist_left = 0;
    double last_dist_right = 0;
    double last_angle = 0;

    double last_time_position = 0;
    double current_time_position = 0;

    // double Kp_linear = 5; // Coefficient proportionnel pour la vitesse linéaire
    // double Kp_angular = 5; // Coefficient proportionnel pour la vitesse angulaire


    static constexpr float Kp_left = 22.66; // Coefficient proportionnel pour le moteur gauche
    static constexpr float Ki_left = 83.64; // Coefficient intégral pour le moteur gauche
    static constexpr float Kp_right = 22.66; // Coefficient proportionnel pour le moteur droit
    static constexpr float Ki_right = 83.64; // Coefficient intégral pour le moteur droit

    
};