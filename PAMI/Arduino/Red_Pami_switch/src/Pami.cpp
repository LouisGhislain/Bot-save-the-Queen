#include "Pami.h"
#include "Motor.h"
#include "Encoder.h"


#define WheelDiameter 0.062 // Diamètre de la roue en mètres
#define DistanceBetweenWheels 0.0996 // Distance entre les roues en mètres
#define MAX_LINEAR_SPEED 0.38 // Vitesse linéaire maximale en m/s
// #define MAX_ANGULAR_SPEED 0.2 // Vitesse angulaire maximale en rad/s
#define Kp_alpha 0.08 // Coefficient proportionnel pour l'angle


PAMI::PAMI() : leftEncoder(2, A4, true), rightEncoder(3, A0, false), leftMotor(5, 7, 8, &leftEncoder, true), rightMotor(6, 10, 11, &rightEncoder, false), tail(9),sonar(A3,4) {
    // Initialisation des moteurs et encodeurs
    leftMotor.set_motor(0);
    rightMotor.set_motor(0);
    // PAMI_state = moving ; 
}


void PAMI::lowlevelcontrol(double ref_speed_left, double ref_speed_right) {

    // Update the speed of the motors
    update_position();

    if (last_time_ctrl == 0) {
        last_time_ctrl = millis() / 1000.0; // Convertir en secondes

    }
    // Update the current time
    current_time_ctrl = millis() / 1000.0; // Convertir en secondes
    double SAMPLING_TIME = current_time_ctrl - last_time_ctrl;

    // Update the last time
    last_time_ctrl = current_time_ctrl;
    

    // Speed error
    double e_speed_left = ref_speed_left - leftMotor.getSpeed();
    double e_speed_right = ref_speed_right - rightMotor.getSpeed();


    // Integrate error
    left_speed += e_speed_left * SAMPLING_TIME;
    right_speed += e_speed_right * SAMPLING_TIME;


    // Calculate control signal
    left_voltage = Kp_left * e_speed_left + Ki_left * left_speed;
    right_voltage = Kp_right * e_speed_right + Ki_right * right_speed;


    if (left_voltage > 9) {
        left_voltage = 9;
    } else if (left_voltage < -9) {
        left_voltage = -9;
    }
    if (right_voltage > 9) {
        right_voltage = 9;
    } else if (right_voltage < -9) {
        right_voltage = -9;
    }


    // Set motor speeds
    leftMotor.set_motor(left_voltage);
    rightMotor.set_motor(right_voltage);

}

void PAMI::pami_brake(){
    leftMotor.brake();
    rightMotor.brake();
}


State_t PAMI::getState() {
    return PAMI_state;
}

void PAMI::setState(State_t new_state) {
    PAMI_state = new_state;
}


double PAMI::getSonarDistance() {
    return sonar.Sonar_Get_Distance();
}

void PAMI::turnTail() {
    tail.Turn_tail();
}


void PAMI::update_position() {
    // Update the distance of the motors
    leftMotor.updateSpeed();
    rightMotor.updateSpeed();

    dist_left = leftMotor.getDistance();
    dist_right = rightMotor.getDistance();

    // Serial.print(millis() - 5000);
    // Serial.print(" , ");
    // Serial.print(leftMotor.getSpeed());
    // Serial.print(" , ");
    // Serial.println(rightMotor.getSpeed());



    angle = 360*(dist_right - dist_left) / (2*PI*DistanceBetweenWheels); // Angle en degrés

    // Update the position of the robot
    x_position = (dist_left + dist_right) / 2.0 * cos(angle * PI / 180.0) + x_start; // Position en x
    y_position = (dist_left + dist_right) / 2.0 * sin(angle * PI / 180.0) + y_start; // Position en y


    
}

double PAMI::getX_position() {
    return x_position;
}
double PAMI::getY_position() {
    return y_position;
}
double PAMI::getAngle() {
    return angle;
}



void PAMI::Turn(double angle_ref) {

    double t_start = millis(); // Temps de départ
    while (millis() - t_start  < 720) {
        // Serial.print(millis()-t_start);
        // Serial.print(" , ");
        // Serial.print(t_start );
        // Serial.print(" , ");
        // Serial.println(millis());
        lowlevelcontrol(0.1, -0.1); // Tourner à gauche
    }
    pami_brake(); // Freiner le robot
    delay(2000); // Attendre 1 seconde avant de continuer
    leftMotor.set_motor(0); // Arrêter le robot
    rightMotor.set_motor(0); // Arrêter le robot
    delay(1000); // Attendre 1 seconde avant de continuer
}


void PAMI::middlecontrol(double x_ref, double y_ref, double angle_ref, bool target, double time) {


    update_position(); // Met à jour la position du robot

    double rho = sqrt(pow(x_ref - x_position, 2) + pow(y_ref - y_position, 2)); // Distance entre la position actuelle et la position de référence

    if (!target_reached) {
        while (rho > 0.08) {
            // Serial.print("Time: ");
            // Serial.println(millis() - time);
            // Serial.print(" , ");
            // Serial.print(millis());
            // Serial.print(" , ");
            // Serial.println(time);
    
            if ((millis() - time) > 99000.0) { // Si le temps écoulé est supérieur à 10 secondes, on arrête le robot
                Serial.println("Time out, stopping the robot");
                lowlevelcontrol(0, 0); // Arrêter le robot
                break;
            }
            else {
                double theta = angle * PI / 180.0; // Convertir l'angle en radians
                rho = sqrt(pow(x_ref - x_position, 2) + pow(y_ref - y_position, 2)); // Distance entre la position actuelle et la position de référence
                double distance_to_ennemy = getSonarDistance(); // Distance entre le robot et l'ennemi
                if (distance_to_ennemy < 15 && distance_to_ennemy > 1) { // Si l'ennemi est trop proche, on arrête le robot
                    Serial.println("Obstacle detected, stopping the robot");
                    // Serial.print("Distance to ennemy: ");
                    // Serial.println(distance_to_ennemy);
                    lowlevelcontrol(0, 0); // Arrêter le robot
                }
                else {
                    double alpha = atan2(y_ref - y_position, x_ref - x_position) - theta; // Angle entre la position actuelle et la position de référence
                    double w = Kp_alpha * alpha; // Vitesse angulai
                    double v = 0.25;

                    double ref_speed_left = (v - w);
                    double ref_speed_right = (v + w);


                    // Limit the speed
                    if (ref_speed_left > MAX_LINEAR_SPEED) {
                        ref_speed_left = MAX_LINEAR_SPEED;
                    } else if (ref_speed_left < -MAX_LINEAR_SPEED) {
                        ref_speed_left = -MAX_LINEAR_SPEED;
                    }
                    if (ref_speed_right > MAX_LINEAR_SPEED) {
                        ref_speed_right = MAX_LINEAR_SPEED;
                    } else if (ref_speed_right < -MAX_LINEAR_SPEED) {
                        ref_speed_right = -MAX_LINEAR_SPEED;
                    }

                    lowlevelcontrol(ref_speed_left, ref_speed_right);

                    // Print the reference speed
                    // Serial.print("Ref speed left: ");
                    // Serial.print(ref_speed_left);
                    // Serial.print(" Ref speed right: ");
                    // Serial.println(ref_speed_right);
                    // Serial.print(" Angle: ");
                    // Serial.print(angle);
                    // Serial.print(" X: ");
                    // Serial.print(x_position);
                    // Serial.print(" Y: ");
                    // Serial.println(y_position);

                    delay(100); // Attendre un peu avant de mettre à jour la position

                    // Serial.print("Distance: ");
                    // Serial.println(rho);
                }
            }
        }
        target_reached = true;
        Serial.println("Target reached");
    }
}

void PAMI::stop() {
    leftMotor.set_motor(0);
    rightMotor.set_motor(0);
}


void PAMI::Rotate(double angle_desired){

    while (abs(angle_desired - angle) > 2){
        double Kp_turn = 0.002; // Coefficient proportionnel pour la rotation
        double angle_error = angle_desired - angle;

        double ref_speed_left = -angle_error * Kp_turn;
        double ref_speed_right = angle_error * Kp_turn;

        lowlevelcontrol(ref_speed_left, ref_speed_right);

        // Serial.print("Angle desired: ");
        // Serial.print(angle_desired);
        // Serial.print(" Angle current: ");
        // Serial.print(angle);
        // Serial.print(" Angle error: ");
        // Serial.println(angle_error);
    }
    pami_brake(); // Freiner le robot
    Serial.println("Rotation terminée");
    Serial.print(angle);
}
