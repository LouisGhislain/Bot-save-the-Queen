#include "Pami.h"
#include "Motor.h"
#include "Encoder.h"


PAMI::PAMI() : leftEncoder(2, A4, true), rightEncoder(3, A0, false), leftMotor(5, 7, 8, &leftEncoder, true), rightMotor(6, 10, 11, &rightEncoder, false), starting_switch(A5), left_switch(1),right_switch(15), tail(9),sonar(A3,4) {
    // Initialisation des moteurs et encodeurs
    leftMotor.set_motor(0);
    rightMotor.set_motor(0);
    PAMI_state = moving ; 
}

// void PAMI::getAngle() {
//     // Récupérer l'angle du robot
//     // Utiliser les encodeurs pour calculer l'angle
//     float leftTicks = leftEncoder.getTicks();
//     float rightTicks = rightEncoder.getTicks();
//     float angle = (leftTicks - rightTicks) / 2.0; // Exemple de calcul d'angle
//     Serial.print("Angle: ");
//     Serial.println(angle);
// }


void PAMI::lowlevelcontrol(double ref_speed_left, double ref_speed_right) {

    // Update the speed of the motors
    leftMotor.updateSpeed();
    rightMotor.updateSpeed();

    if (last_time_ctrl == 0) {
        last_time_ctrl = millis() / 1000.0; // Convertir en secondes

    }
    // Update the current time
    current_time_ctrl = millis() / 1000.0; // Convertir en secondes
    float SAMPLING_TIME = current_time_ctrl - last_time_ctrl;

    // Update the last time
    last_time_ctrl = current_time_ctrl;
    

    // Speed error
    double e_speed_left = ref_speed_left - leftMotor.getSpeed();
    double e_speed_right = ref_speed_right - rightMotor.getSpeed();

    // Print the time
    // Serial.print("Time: ");
    Serial.print(current_time_ctrl);
    Serial.print(",");

    
//    // Print the error
    // Serial.print("e_speed_left: ");
    Serial.print(e_speed_left);
    Serial.print(",");
    // Serial.print(" e_speed_right: ");
    Serial.print(e_speed_right);
    Serial.print(",");


    // Integrate error
    left_speed += e_speed_left * SAMPLING_TIME;
    right_speed += e_speed_right * SAMPLING_TIME;

    // Serial.print(left_speed);
    // Serial.print(",");
    // Serial.print(right_speed);
    // Serial.print(",");


    // Calculate control signal
    left_voltage = Kp_left * e_speed_left + Ki_left * left_speed;
    right_voltage = Kp_right * e_speed_right + Ki_right * right_speed;

    Serial.print(Kp_left*e_speed_left);
    Serial.print(",");
    Serial.print(Ki_left*left_speed);
    Serial.print(",");


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

    leftMotor.set_motor(left_voltage);
    rightMotor.set_motor(right_voltage);

        // Set motor speeds
    // Serial.print("Left voltage: ");
    Serial.print(left_voltage);
    Serial.print(",");
    // Serial.print(" Right voltage: ");
    Serial.println(right_voltage);

    //Print the speed and the target speed
    // Serial.print("Left speed: ");
    // Serial.print(leftMotor.getSpeed());
    // Serial.print(" Target speed: ");
    // Serial.print(ref_speed_left);
    // Serial.print(" Right speed: ");
    // Serial.print(rightMotor.getSpeed());
    // Serial.print(" Target speed: ");
    // Serial.println(ref_speed_right);
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

bool PAMI::isStartPressed() {
    return starting_switch.switch_state();
}

bool PAMI::isLeftPressed() {
    return left_switch.switch_state();
}

double PAMI::getSonarDistance() {
    return sonar.Sonar_Get_Distance();
}

void PAMI::turnTail() {
    tail.Turn_tail();
}
