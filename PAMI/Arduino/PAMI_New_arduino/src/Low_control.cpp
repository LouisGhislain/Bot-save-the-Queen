// #include <Arduino.h>   
// #include "Pami.h"

// void PAMI::lowlevelcontrol(double ref_speed_left, double ref_speed_right) {

//     // Update the current time
//     current_time_ctrl = millis() / 1000.0; // Convertir en secondes
//     float SAMPLING_TIME = current_time_ctrl - last_time_ctrl;
    

//     // Speed error
//     double e_speed_left = ref_speed_left - leftMotor.getSpeed();
//     double e_speed_right = ref_speed_right - rightMotor.getSpeed();


//     // Integrate error
//     left_speed += e_speed_left * SAMPLING_TIME;
//     right_speed += e_speed_right * SAMPLING_TIME;

//     // Calculate control signal
//     left_voltage = Kp_left * e_speed_left + Ki_left * left_position;
//     right_voltage = Kp_right * e_speed_right + Ki_right * right_position;

//     // Set motor speeds
//     leftMotor.set_motor(left_voltage);
//     rightMotor.set_motor(right_voltage);
// }