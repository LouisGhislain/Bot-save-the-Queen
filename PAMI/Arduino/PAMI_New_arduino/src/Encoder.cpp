#include <Arduino.h>
#include "Encoder.h"

// Constants
int PPR = 13;
int Gear_ratio = 42;
int PPR_total = PPR * Gear_ratio;

// Pin definitions
#define PHASE_A_LEFT 2  // Pin 2 pour interruptions
#define PHASE_B_LEFT 4  
#define PHASE_A_RIGHT 3  // Pin 3 pour interruptions
#define PHASE_B_RIGHT 5  

// Variables
volatile int leftTicks = 0;
volatile int rightTicks = 0;

// Function to reset the left encoder count
void resetLeftEncoder() {
    leftTicks = 0;
}

// Function to reset the right encoder count
void resetRightEncoder() {
    rightTicks = 0;
}

// Pin Initialization
void Pin_Encoder_Initialization() {
    pinMode(PHASE_A_LEFT, INPUT_PULLUP);
    pinMode(PHASE_B_LEFT, INPUT_PULLUP);
    pinMode(PHASE_A_RIGHT, INPUT_PULLUP);
    pinMode(PHASE_B_RIGHT, INPUT_PULLUP);
    resetLeftEncoder();
    resetRightEncoder();
    attachInterrupt(digitalPinToInterrupt(PHASE_A_LEFT), leftEncoder, RISING);
    attachInterrupt(digitalPinToInterrupt(PHASE_A_RIGHT), rightEncoder, RISING);
}

// Interrupt Service Routine for the left encoder
void leftEncoder() {
    if (digitalRead(PHASE_B_LEFT) == LOW) {
        leftTicks++;
    } else {
        leftTicks--;
    }
}

// Interrupt Service Routine for the right encoder
void rightEncoder() {
    if (digitalRead(PHASE_B_RIGHT) == LOW) {
        rightTicks++;
    } else {
        rightTicks--;
    }
}

// Function to get the left encoder count
int getLeftEncoder() {
    return leftTicks;
}

// Function to get the right encoder count
int getRightEncoder() {
    return rightTicks;
}

// Function to get the left encoder count in radians
float getLeftEncoderRad() {
    return (leftTicks * 2.0 * PI) / PPR_total;
}

// Function to get the right encoder count in radians
float getRightEncoderRad() {
    return (rightTicks * 2.0 * PI) / PPR_total;
}