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
    teensy_init(); //init I2C teensy
    digitalWrite(22, LOW); 
    //screen_init(); //init I2C screen
    initialize_pins();
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
    unsigned char resetCommand[5] = {0x7F, 0x00, 0x00, 0x00, 0x00};
    wiringPiSPIDataRW(SPI_CHANNEL, resetCommand, sizeof(resetCommand));
    
    // Check if reset was successful (should see 0x7F in the response)
    // if (resetCommand[1] == 0x7F) {
    //     std::cout << "Encoder values and coordinates reset successfully." << std::endl;
    // } else {
    //     std::cout << "Reset command may have failed!" << std::endl;
    // }
}
void Robot::initialize_pins() {
    wiringPiSetupGpio();  // Use BCM numbering
    pinMode(STARTING_CORD_PIN, INPUT);
    pinMode(MICROSWITCH_BACK_LEFT, INPUT);
    pinMode(MICROSWITCH_BACK_RIGHT, INPUT);
    // Set the buzzer pin as output
    pinMode(BUZZER_PIN, OUTPUT);
}