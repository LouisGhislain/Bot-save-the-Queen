#include "../../include/Teensy.h"

void Teensy::secondStage(){

    myHoldCans.releaseExternal();
    myHoldCans.grabCenter();
    delay(500);
    myPushPlank.routine_separation_stack();
    delay(1000);
    myLift.up_and_down(secondStageHeight + 10);
    delay(1000);
    myHoldCans.releaseCenter();
    delay(1000);
    myLift.up_and_down(floorHeight);
    delay(1000);

}