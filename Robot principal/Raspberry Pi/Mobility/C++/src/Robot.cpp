// Robot.cpp
#include "Robot.h"
#include <fstream>

Robot::Robot() 
    : leftMotor(13, 16, 7, 0x12, 0x10, false), // (pwmPin, forwardDirectionPin, backwardDirectionPin, distanceAddress, speedAddress, baseDir)
      rightMotor(12, 23, 27, 0x13, 0x11, true) {
    wiringPiSetup();
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
 * @brief Start the robot
 * 
 * This function initializes the motors and resets the integral error terms.
 * 
 */
void Robot::start() {
    initializeSPI();
    resetValues();

    //leftMotor.start();       TO TRY : They should start at initialization of the robot, else : uncomment these lines
    //rightMotor.start();
    
    intEPosLeft = intEPosRight = 0.0;
    intESpeedLeft = intESpeedRight = 0.0;
}

/**
 * @brief Stop the robot
 * 
 * This function stops the motors.
 */
void Robot::stop() {
    leftMotor.brake();
    rightMotor.brake();
    usleep(10000000);
    leftMotor.stop();
    rightMotor.stop();
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

/**
 * @brief Get the angle of the robot
 * 
 * Computes it based on the difference in distance between the left and right motors.
 * 
 * @return double Angle in degrees
 */
double Robot::getAngle() const {
    double theta = 360 * (rightMotor.getDistance() - leftMotor.getDistance()) / (2 * M_PI * 23.65);
    return theta;
}

/**
 * @brief Low-level controller - Imposes a speed to the motors
 * 
 * This function implements a PI controller for the motors by modifying directly the speed of the motors.
 * Feeds into the setSpeed function of the Motor class.
 * 
 * @param ref_speed_left Reference speed for the left motor
 * @param ref_speed_right Reference speed for the right motor
 */
void Robot::lowLevelController(double ref_speed_left, double ref_speed_right) {
    // Speed error
    double e_speed_left = ref_speed_left - leftMotor.getSpeed();
    double e_speed_right = ref_speed_right - rightMotor.getSpeed();

    // Integrate error
    intESpeedLeft += e_speed_left * SAMPLING_TIME;
    intESpeedRight += e_speed_right * SAMPLING_TIME;

    // Calculate control signal
    u_volt_left = KpSpeed * e_speed_left + KiSpeed * intESpeedLeft;
    u_volt_right = KpSpeed * e_speed_right + KiSpeed * intESpeedRight;

    // Set motor speeds
    leftMotor.setSpeed(u_volt_left);
    rightMotor.setSpeed(u_volt_right);
}

/**
 * @brief Middle-level controller - Go to a position
 * 
 * This function implements a middle-level controller that creates a trajectory to a target position.
 * 
 * @param x X-coordinate of the target position
 * @param y Y-coordinate of the target position
 * @param goal_angle Angle to the target position
 */
void Robot::middleLevelController(double x, double y, double goal_angle) {
// Initial distance value
double rho = 1000;

while (rho > 10) {
    // Calculate distance to target (distance error)
    rho = std::sqrt(std::pow(x - xCoord, 2) + std::pow(y - yCoord, 2)); // Distance in cm

    // Calculate angle to target (angle error)
    double alpha = std::atan2(y - yCoord, x - xCoord) * (360 / (2 * M_PI)) - getAngle(); // Angle in degrees

    double beta = goal_angle - getAngle() - alpha; // Angle in degrees

    // Calculate speed references
    double v = KpRho * rho + 0.2;
    double w = KpAlpha * alpha + KpBeta * beta;
    double ref_speed_left = v - w;
    double ref_speed_right = v + w;

    // Control loop
    lowLevelController(ref_speed_left, ref_speed_right);

    // Update position
    double displacement = (leftMotor.getDistance() - lastLeftDistance + rightMotor.getDistance() - lastRightDistance) / 2;

    xCoord += displacement * std::cos((getAngle() / 360) * 2 * M_PI) * (100 / 70.2);
    yCoord += displacement * std::sin((getAngle() / 360) * 2 * M_PI) * (100 / 70.2);

    lastLeftDistance = leftMotor.getDistance();
    lastRightDistance = rightMotor.getDistance();

    // Sleep (equivalent to Python's sleep)
    usleep(SAMPLING_TIME * 1e6); // Convert to microseconds
    }

stop();
}

/**
 * @brief Routine to pass through predefined points
 * 
 * This function makes the robot move through points using the middleLevelController.
 */
void Robot::routine() {
    // Define 3 target points (x, y, angle in degrees)
    std::vector<std::tuple<double, double, double>> points = {
        {100.0, -100.0, 0.0},   // Point 1: 100 cm forward, angle 0 degrees
        {200.0, 0, 0.0}, // Point 2: 100 cm to the right, angle 90 degrees
        {200.0, -120.0, 0.0}   // Point 3: 100 cm backward, angle 180 degrees
    };

    // Loop through each point and move to it
    for (const auto& point : points) {
        double x = std::get<0>(point);
        double y = std::get<1>(point);
        double angle = std::get<2>(point);

        std::cout << "Moving to point: (" << x << ", " << y << ") with angle: " << angle << " degrees" << std::endl;
        middleLevelController(x, y, angle);
    }

    std::cout << "Routine completed!" << std::endl;
}

void Robot::openLoopData() {
    // Open file
    std::ofstream file;
    file.open("encoder_data.txt", std::ios::out);
    
    if (!file) {
        std::cout << "Erreur ouverture fichier" << std::endl;
        return;
    }
    file << "0, " << leftMotor.getSpeed() << ", " << rightMotor.getSpeed() << "\n";
    
    leftMotor.setSpeed(12);
    rightMotor.setSpeed(12);
    
    unsigned long startTime = micros(); // Current time in µs
    unsigned long currentTime;
    
    // Data taken every 100 microseconds 
    unsigned long duration = 3000000; 
    while (micros() - startTime < duration) {
        currentTime = micros() - startTime;
        
        //Save data every 100 microseconds
        if (currentTime % 1000 == 0) {
            file << currentTime << ", " 
                 << leftMotor.getSpeed() << ", " 
                 << rightMotor.getSpeed() << "\n";
        }
    }

    leftMotor.stop();
    rightMotor.stop();
    file.close();
    std::cout << "Test completed" << std::endl;
}

void Robot::lowLevelTest(){
    // Open file
    std::ofstream file;
    file.open("closedloop_data.txt", std::ios::out);
    
    if (!file) {
        std::cout << "Erreur ouverture fichier" << std::endl;
        return;
    }
    file << "0, " << leftMotor.getSpeed() << ", " << rightMotor.getSpeed() << ", " << u_volt_left << ", " << u_volt_right << "\n";

    double ref_speed_left = 30;
    double ref_speed_right = 30;

    unsigned long startTime = micros(); // Current time in µs
    unsigned long duration = 1000000;
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
    stop();
    file.close();
    std::cout << "Test completed" << std::endl;
}

void Robot::printDistance(){
    while(true){
    std::cout<< "Left distance: "<< leftMotor.getDistance() << "     Right distance: "<< rightMotor.getDistance() << std::endl;
    }
}


#define BUZZER_PIN 22  // GPIO22 corresponds to WiringPi pin 3
void Robot::buzzBuzzer(){
    // Set the buzzer pin as output
    pinMode(BUZZER_PIN, OUTPUT);

    std::cout << "Buzzer test started. Press Ctrl+C to exit." << std::endl;

    // Turn the buzzer on
    digitalWrite(BUZZER_PIN, HIGH);
    std::cout << "Buzzer ON" << std::endl;
    delay(1000);  // Wait for 1 second

    // Turn the buzzer off
    digitalWrite(BUZZER_PIN, LOW);
    std::cout << "Buzzer OFF" << std::endl;
}

#define STATE1_PIN 6
#define STATE2_PIN 19

void Robot::teensy_cans(){  
    pinMode(STATE1_PIN, OUTPUT);
    pinMode(STATE2_PIN, OUTPUT);

    //Sending 01 on the teensy
    digitalWrite(STATE1_PIN, HIGH);
    digitalWrite(STATE2_PIN, LOW);
    delay(10000);
    //Turn off
    digitalWrite(STATE1_PIN, LOW);
    std::cout << "Stop sending" << std::endl ;
}

void Robot::teensy_lift(){  
    pinMode(STATE1_PIN, OUTPUT);
    pinMode(STATE2_PIN, OUTPUT);
    
    //Sending 10 on the teensy
    digitalWrite(STATE1_PIN, LOW);
    digitalWrite(STATE2_PIN, HIGH);
    delay(5000);
    //Turn off
    digitalWrite(STATE2_PIN, LOW);
    std::cout << "Stop sending" << std::endl ;
}

void Robot::teensy_cans_lift(){  
    pinMode(STATE1_PIN, OUTPUT);
    pinMode(STATE2_PIN, OUTPUT);
    
    //Sending 11 on the teensy
    digitalWrite(STATE1_PIN, HIGH);
    digitalWrite(STATE2_PIN, HIGH);
    delay(5000);
    //Turn off
    digitalWrite(STATE1_PIN, LOW);
    digitalWrite(STATE2_PIN, LOW);
    std::cout << "Stop sending" << std::endl ;
}
