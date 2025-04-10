#include "../../include/Teensy.h"

void Teensy::clear(){
    myPushPlank.begin();
    myHoldCans.begin();
    myLift.begin();

    myHoldCans.releaseAll();
    myPushPlank.pull_plank_release();
}
