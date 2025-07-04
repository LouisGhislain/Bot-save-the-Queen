#include "../../../include/Motor.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <wiringPi.h>

Motor::Motor(int pwmPin, int forwardDirectionPin, int backwardDirectionPin, uint8_t distanceAddress, uint8_t speedAddress, bool baseDir, double odometerDiameter)
    : pwmPin(pwmPin), forwardDirectionPin(forwardDirectionPin), backwardDirectionPin(backwardDirectionPin), distanceAddress(distanceAddress), speedAddress(speedAddress), baseDir(baseDir), odometerDiameter(odometerDiameter){
    
    // Initialize GPIO
    wiringPiSetupGpio();  // Use BCM numbering
    pinMode(pwmPin, PWM_OUTPUT);
    pinMode(forwardDirectionPin, OUTPUT);
    pinMode(backwardDirectionPin, OUTPUT);

    // Set PWM range and divider
    pwmSetMode(PWM_MODE_MS); // Mark:Space mode
    pwmSetClock(2); // 
    pwmSetRange(480); // 
    pwmWrite(pwmPin, 0); // Set duty cycle to 0%
}

int32_t Motor::readData(const std::string& type) const {
    // if command == "distance" then read distance, else read speed
    uint8_t readCommand[5] = { (type == "distance") ? distanceAddress : speedAddress, 0x00, 0x00, 0x00, 0x00 };

    wiringPiSPIDataRW(SPI_CHANNEL, readCommand, 5);

    return (int32_t)((readCommand[1] << 24) | (readCommand[2] << 16) | (readCommand[3] << 8) | readCommand[4]);
}

/*
* @brief Get the speed of the motor in rad/s
*
* @return Speed of the motor in rad/s
*/
double Motor::getSpeed() const {
    int32_t ticks_per_1ms = readData("speed");

    double speed = (ticks_per_1ms / static_cast<double>(ENCODER_COUNTS_PER_REV))*1000*2*M_PI; // Speed in rad/s
    return speed;
}


/*
* @brief Get the distance travelled by the odometer in m
*
* @return Distance travelled by the odometer in m
*/
double Motor::getDistance() const {
    int32_t ticks = readData("distance");
    double distance = -(ticks * M_PI * odometerDiameter) / (TICKS_COUNT_AMT_103) * 1.009748223;  // Corrective factor
    return distance;
}

/*
* @brief Compute the back EMF of the motor based on its speed
*
* @return Back EMF of the motor in V
*/
double Motor::getBackEMF() const {
    double BackEMF = this->getSpeed() * kPhiOfMotor * GEAR_RATIO; // Back EMF in V
    return BackEMF;
}

void Motor::setVoltage(double voltage) {
    voltage = std::clamp(voltage, -VOLTAGE_LIMIT, VOLTAGE_LIMIT);
    double BackEMF = this->getBackEMF();
    double dutyCycle = (voltage-BackEMF)/(24-BackEMF) ; // Duty cycle in [0, 1]

    if (dutyCycle < 0) {
        digitalWrite(forwardDirectionPin, !baseDir);  // Backward
        digitalWrite(backwardDirectionPin, baseDir);
    } else {
        digitalWrite(forwardDirectionPin, baseDir);   // Forward
        digitalWrite(backwardDirectionPin, !baseDir);
    }

    pwmWrite(pwmPin, 480*(fabs(dutyCycle)));
}

void Motor::stop() {
    pwmWrite(pwmPin, 0); // Set duty cycle to 0%
    //std::cout << "Motor stopped" << std::endl;
}

void Motor::brake() {
    digitalWrite(forwardDirectionPin, true);  // Active braking (both direction pins on high)
    digitalWrite(backwardDirectionPin, true);
    pwmWrite(pwmPin, 480); // Set duty cycle to 100%
    //std::cout << "Motor braking" << std::endl;
}

void Motor::start() {
    
    pwmWrite(pwmPin, 0);
}
