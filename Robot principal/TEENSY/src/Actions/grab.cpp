#include "../../include/Teensy.h"

void Teensy::grab(){
    myLift.up_and_down(floorHeight-10);
    myHoldCans.grabAll();
    delay(200);
    myLift.up_and_down(floorHeight+7); //1cm 
    delay(200);
    myPushPlank.pull_plank_grab();
    delay(100);
    Wire.write(CMD_GRAB_DONE);
}
