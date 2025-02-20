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
    int directionPin;
    uint8_t distanceAddress;
    uint8_t speedAddress;
    bool baseDir;

public:
    static constexpr int MOTOR_PWM_FREQUENCY = 1120;
    static constexpr int GEAR_RATIO = 30;   
    static constexpr int ENCODER_COUNTS_PER_REV = 4*64*GEAR_RATIO;
    static constexpr double VOLTAGE_LIMIT = 12.0;
    
    Motor(int pwmPin, int directionPin, uint8_t distanceAddress, uint8_t speedAddress, bool baseDir);
    
    int32_t readData(const std::string& type) const;
    double getSpeed() const;
    double getDistance() const;
    void setSpeed(double voltage);
    void stop();
    void start();
};
