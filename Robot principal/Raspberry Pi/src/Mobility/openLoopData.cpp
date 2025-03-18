#include "../../include/Robot.h"
#include <fstream>

/**
 * @brief Collects open loop data
 * 
 * Applies a fixed duty cycle to the motors and measures the speed for a certain amount of time
 * Writes collected data into encoder_data.txt
 * 
 * Format of the data: time, left motor speed, right motor speed
 * 
 */
void Robot::openLoopData() {
    // Open file
    std::ofstream file;
    file.open("encoder_data.txt", std::ios::out);
    
    if (!file) {
        std::cout << "Erreur ouverture fichier" << std::endl;
        return;
    }
    file << "0, " << leftMotor.getSpeed() << ", " << rightMotor.getSpeed() << "\n";
    
    leftMotor.setSpeed(5.0);
    rightMotor.setSpeed(5.0);
    
    unsigned long startTime = micros(); // Current time in µs
    unsigned long currentTime;
    
    // Data taken every 100 microseconds 
    unsigned long duration = 10*1000000; // To seconds; 
    while (micros() - startTime < duration) {
        currentTime = micros() - startTime;
        
        //Save data every 100 microseconds
        if (currentTime % 1000 == 0) {
            file << currentTime << ", " << leftMotor.getSpeed() << ", " << rightMotor.getSpeed() << "\n";
        }
    }

    leftMotor.stop();
    rightMotor.stop();
    file.close();
    std::cout << "Test completed" << std::endl;
}