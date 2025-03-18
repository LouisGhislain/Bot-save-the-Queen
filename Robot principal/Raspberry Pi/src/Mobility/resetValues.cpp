#include "../../include/Robot.h"

/**
 * @brief Reset encoder values and coordinates
 * 
 * This function sends a reset command to the encoder and resets the coordinates and distance values.
 */
void Robot::resetValues() {
    // Send reset command to encoder
    static unsigned char resetCommand[5] = {0x7F, 0x00, 0x00, 0x00, 0x00};
    wiringPiSPIDataRW(SPI_CHANNEL, resetCommand, sizeof(resetCommand));    

    // Reset coordinates and distance values
    xCoord = 0;
    yCoord = 0;
    lastLeftDistance = 0;
    lastRightDistance = 0;
    intEPosLeft = intEPosRight = 0.0;
    intESpeedLeft = intESpeedRight = 0.0;

    // Print confirmation message
    std::cout << "Encoder values and coordinates reset." << std::endl;
}