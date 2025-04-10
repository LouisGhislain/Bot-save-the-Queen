// Teensy.cpp

#include "Teensy.h"

Teensy::Teensy() {
    myPushPlank.begin();
    myHoldCans.begin();
    myLift.begin();
    myHoldCans.releaseAll();
}