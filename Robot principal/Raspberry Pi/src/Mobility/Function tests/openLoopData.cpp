#include "../../../include/Robot.h"
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
    
    usleep(1000000); // Wait for 1 second


    unsigned long currentTime;
    
    double robotspeed;
    double backemf;
    double previoustime = 0.0;


    double integratedDistance= 0.0;
    unsigned long startloop;
    unsigned long looptime;

    unsigned long startTime = micros(); // Current time in µs

    // while (integratedDistance<2*M_PI){
    //     startloop = micros();
    //     integratedDistance += ((rightMotor.getSpeed()))*(0.001); // in m
    //     fprintf(stderr, "Integrated distance: %f\n", integratedDistance);
    //     looptime = micros() - startloop;
    //     usleep(0.001*1e6 - looptime);
    // }
    // leftMotor.setSpeed(0);
    
    //Data taken every millisecond 
    unsigned long duration = 100*1000000; // To seconds; 
    while (micros() - startTime < duration) {
        rightMotor.setSpeed(6);
        leftMotor.setSpeed(6);
        currentTime = micros() - startTime;

        //Save data every millisecond
        if (currentTime-previoustime > 1000) {
            //file << currentTime << ", " << leftMotor.getSpeed() << ", " << rightMotor.getSpeed() << "\n";
            fprintf(stderr, "Time: %lu, Left speed: %f, Right speed: %f\n", currentTime, leftMotor.getSpeed(), rightMotor.getSpeed());
            previoustime = currentTime;
        }
    }

    leftMotor.stop();
    rightMotor.stop();
    file.close();
    std::cout << "Test completed" << std::endl;
}