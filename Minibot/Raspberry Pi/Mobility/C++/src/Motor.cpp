#include "Motor.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <softPwm.h>

Motor::Motor(int pwmPin, int directionPin, uint8_t distanceAddress, uint8_t speedAddress, bool baseDir)
    : pwmPin(pwmPin), directionPin(directionPin), distanceAddress(distanceAddress), speedAddress(speedAddress), baseDir(baseDir){
    
    // Initialize GPIO
    wiringPiSetupGpio();  // Use BCM numbering
    pinMode(pwmPin, PWM_OUTPUT);
    pinMode(directionPin, OUTPUT);

    // Start PWM with 0% duty cycle
    softPwmCreate(pwmPin, 0, 100);
}

int32_t Motor::readData(const std::string& type) {
    uint8_t readCommand[5] = { (type == "distance") ? distanceAddress : speedAddress, 0x00, 0x00, 0x00, 0x00 };
    uint8_t response[5] = {0};

    wiringPiSPIDataRW(1, readCommand, 5);
    std::memcpy(response, readCommand, 5);  // SPI response is in the same buffer

    return (int32_t)((response[1] << 24) | (response[2] << 16) | (response[3] << 8) | response[4]);
}

double Motor::getSpeed() {
    int32_t ticks_per_10ms = readData("speed");
    double diameter = 6.0325;  // cm
    return (ticks_per_10ms / static_cast<double>(ENCODER_COUNTS_PER_REV)) * M_PI * diameter * 4;
}

double Motor::getDistance() {
    int32_t ticks = readData("distance");
    return (ticks * 3.14159 * 4.5) / (4.0 * 2048.0) * 1.01617;  // Corrective factor
}

void Motor::setSpeed(double voltage) {
    voltage = std::max(-VOLTAGE_LIMIT, std::min(VOLTAGE_LIMIT, voltage));
    int dutyCycle = static_cast<int>(100 * std::abs(voltage) / VOLTAGE_LIMIT);

    if (voltage < 0) {
        digitalWrite(directionPin, !baseDir);  // Backward
    } else {
        digitalWrite(directionPin, baseDir);   // Forward
    }

    softPwmWrite(pwmPin, dutyCycle);
}

void Motor::stop() {
    softPwmWrite(pwmPin, 0);
}

void Motor::start() {
    softPwmWrite(pwmPin, 0);
}
