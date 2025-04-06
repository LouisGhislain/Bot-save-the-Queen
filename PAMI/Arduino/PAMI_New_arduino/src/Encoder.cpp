#include "Encoder.h"

Encoder* Encoder::leftInstance = nullptr;
Encoder* Encoder::rightInstance = nullptr;


Encoder::Encoder(int pinA, int pinB, bool isLeft) : PHASE_A(pinA), PHASE_B(pinB), ticks(0) {
    pinMode(PHASE_A, INPUT_PULLUP);
    pinMode(PHASE_B, INPUT_PULLUP);

    if (isLeft) {
        leftInstance = this;
        attachInterrupt(digitalPinToInterrupt(PHASE_A), handleLeftInterrupt, RISING);
    } else {
        rightInstance = this;
        attachInterrupt(digitalPinToInterrupt(PHASE_A), handleRightInterrupt, RISING);
    }
}

void Encoder::handleInterruptRight() {
    if (digitalRead(PHASE_B) == LOW) {
        ticks++;
    } else {
        ticks--;
    }
}

void Encoder::handleInterruptLeft() {
    if (digitalRead(PHASE_B) == LOW) {
        ticks--;
    } else {
        ticks++;
    }
}

void Encoder::reset() {
    ticks = 0;
}

int Encoder::getTicks() {
    return ticks;
}

float Encoder::getRadians() {
    int totalPPR = PPR * gearRatio;
    return (ticks * 2.0 * PI) / totalPPR;
}

void Encoder::handleLeftInterrupt() {
    if (leftInstance != nullptr) {
        leftInstance->handleInterruptLeft();
    }
}

void Encoder::handleRightInterrupt() {
    if (rightInstance != nullptr) {
        rightInstance->handleInterruptRight();
    }
}