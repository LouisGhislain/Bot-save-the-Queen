#include "../../include/Robot.h"
#include <fstream>

/**
 * @brief Test the low level controller
 * 
 * Imposes a reference speed to the motors and measures the speed for a certain amount of time
 * Writes collected data into closedloop_data.txt
 * 
 * Format of the data: time, left motor speed, right motor speed, left motor voltage, right motor voltage
 * 
 */
void Robot::lowLevelTest(){
    // Open file
    std::ofstream file;
    file.open("closedloop_data.txt", std::ios::out);
    
    if (!file) {
        std::cout << "Erreur ouverture fichier" << std::endl;
        return;
    }
    file << "0, " << leftMotor.getSpeed() << ", " << rightMotor.getSpeed() << ", " << u_volt_left << ", " << u_volt_right << "\n";

    double ref_speed_left = 13.5;
    double ref_speed_right = 13.5;

    unsigned long startTime = micros(); // Current time in µs
    unsigned long duration = 3*1000000; // To seconds;
    unsigned long currentTime;

    while (micros() - startTime < duration) {
        currentTime = micros() - startTime;
        lowLevelController(ref_speed_left, ref_speed_right);
        usleep(SAMPLING_TIME * 1e6); // Convert to microseconds
        file << currentTime << ", " 
                << leftMotor.getSpeed() << ", " 
                << rightMotor.getSpeed() << ", "
                << u_volt_left << ", " 
                << u_volt_right << "\n";
    }
    /*
    ref_speed_left = 60;
    ref_speed_right = 60;
    while (micros() - startTime < 2*duration) {
        currentTime = micros() - startTime;
        lowLevelController(ref_speed_left, ref_speed_right);
        usleep(SAMPLING_TIME * 1e6); // Convert to microseconds
        file << currentTime << ", " 
                << leftMotor.getSpeed() << ", " 
                << rightMotor.getSpeed() << ", "
                << u_volt_left << ", " 
                << u_volt_right << "\n";
    }
    */
    stop();
    file.close();
    std::cout << "Test completed" << std::endl;
}