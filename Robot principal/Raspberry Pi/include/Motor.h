#pragma once

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <cmath>
#include <cstdint>
#include <string>
#include <algorithm>    


class Motor {
private:
    int pwmPin;
    int forwardDirectionPin;
    int backwardDirectionPin;
    uint8_t distanceAddress;
    uint8_t speedAddress;
    bool baseDir;
    
    // SPI Constants
    static constexpr int SPI_CHANNEL = 0;
    static constexpr int SPI_SPEED = 500000;

public:
    static constexpr int MOTOR_PWM_FREQUENCY = 30000;
    static constexpr int GEAR_RATIO = 676/49; // Faulhaber serie number 23/1
    static constexpr double TICKS_COUNT_AMT_103 = 8192;  // AMT103 8192 = 4*2048
    static constexpr int ENCODER_COUNTS_PER_REV = TICKS_COUNT_AMT_103 * GEAR_RATIO;
    static constexpr double WHEEL_DIAMETER = 6.0325;  // cm
    static constexpr double ODOMETER_DIAMETER = 0.0445;  // m

    static constexpr double VOLTAGE_LIMIT = 24.0;
    
    Motor(int pwmPin, int forwardDirectionPin, int backwardDirectionPinwardDirectionPin, uint8_t distanceAddress, uint8_t speedAddress, bool baseDir);
    
    int32_t readData(const std::string& type) const;
    double getSpeed() const;
    double getDistance() const;
    void setSpeed(double voltage);
    void stop();
    void start();
    void brake();
};
