// Robot.cpp
#include "Robot.h"

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
    double u_volt_left = KpSpeed * e_speed_left + KiSpeed * intESpeedLeft;
    double u_volt_right = KpSpeed * e_speed_right + KiSpeed * intESpeedRight;

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

void Robot::testMotors() {
    lowLevelController(1, 1);
    usleep(2000000);  // 2 seconds
    //Print speed
    std::cout << "Left motor speed: " << leftMotor.getSpeed() << std::endl;
    std::cout << "Right motor speed: " << rightMotor.getSpeed() << std::endl;
    usleep(1000000);  // 1 seconds
    lowLevelController(-1, -1);
    usleep(2000000);  // 2 seconds
    //Print speed
    std::cout << "Left motor speed: " << leftMotor.getSpeed() << std::endl;
    std::cout << "Right motor speed: " << rightMotor.getSpeed() << std::endl;
    usleep(1000000);  // 1 seconds
    leftMotor.stop();
    rightMotor.stop();
}

void Robot::printDistance(){
    std::cout<< "Left distance: "<< leftMotor.getDistance() << std::endl;
    std::cout<< "Right distance: "<< rightMotor.getDistance() << std::endl;
}



