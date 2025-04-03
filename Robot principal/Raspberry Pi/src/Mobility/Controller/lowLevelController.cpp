#include "../../../include/Robot.h"

/**
 * @brief Low-level controller - Imposes a speed to the motors
 * 
 * This function implements a PI controller for the motors by modifying directly the speed of the motors.
 * Feeds into the setVoltage function of the Motor class.
 * 
 * @param ref_speed_left Reference speed for the left motor
 * @param ref_speed_right Reference speed for the right motor
 */
void Robot::lowLevelController(double ref_speed_left, double ref_speed_right) {
    double current_speed_left = leftMotor.getSpeed();
    double current_speed_right =  rightMotor.getSpeed();

    // Speed error
    double e_speed_left = ref_speed_left - current_speed_left;
    double e_speed_right = ref_speed_right - current_speed_right;

    // Integrate error
    intESpeedLeft += e_speed_left * SAMPLING_TIME;
    intESpeedRight += e_speed_right * SAMPLING_TIME;

    // Calculate control signal
    u_volt_left = KpSpeed * e_speed_left + KiSpeed * intESpeedLeft;
    u_volt_right = KpSpeed * e_speed_right + KiSpeed * intESpeedRight;

    // Set motor speeds
    leftMotor.setVoltage(u_volt_left);
    rightMotor.setVoltage(u_volt_right);
}