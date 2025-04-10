#include "../../include/Teensy.h"

void Teensy::grab(){
    grabInProgress = true;
    myHoldCans.grabAll();
    delay(200);
    myLift.up_and_down(98); //2cm 
    delay(200);
    myPushPlank.pull_plank_grab();
    delay(100);
    grabInProgress = false ; 
    Wire.write(CMD_GRAB_DONE);
}
