#pragma once
#include <Arduino.h>

class Encoder {
private:
    int PHASE_A;
    int PHASE_B;
    volatile int ticks;


    int PPR = 13;
    int gearRatio = 42;

    static Encoder* leftInstance;
    static Encoder* rightInstance;

public:

    Encoder(int pinA, int pinB, bool isLeft = false);
    void handleInterruptLeft();
    void handleInterruptRight();


    void reset();
    int getTicks();
    double getRadians();

    static void handleLeftInterrupt();
    static void handleRightInterrupt();

};
