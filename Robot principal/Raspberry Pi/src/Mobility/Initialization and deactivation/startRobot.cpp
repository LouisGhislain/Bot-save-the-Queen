#include "../../../include/Robot.h"

/**
 * @brief Start the robot
 * 
 * This function initializes the motors and resets the integral error terms.
 * 
 */
void Robot::start() {
    initializeSPI();
    resetValues();
    intEPosLeft = intEPosRight = 0.0;
    intESpeedLeft = intESpeedRight = 0.0;
}

/**
 * @brief Initialize SPI communication
 */
void Robot::initializeSPI() {
    if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1) { // (SPI CHANNEL, SPI SPEED)
        throw std::runtime_error("Failed to initialize SPI");
    }
}

/**
 * @brief Reset encoder values and coordinates
 * 
 * This function sends a reset command to the encoder and resets the coordinates and distance values.
 */
void Robot::resetValues() {
    // Send reset command to encoder
    static unsigned char resetCommand[5] = {0x7F, 0x00, 0x00, 0x00, 0x00};
    wiringPiSPIDataRW(SPI_CHANNEL, resetCommand, sizeof(resetCommand));    

    // Print confirmation message
    //std::cout << "Encoder values and coordinates reset." << std::endl;
}