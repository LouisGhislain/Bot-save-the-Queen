#include "../../include/Teensy.h"

void Teensy::build( ){
    //Lift down
    myLift.up_and_down(floorHeight); 

    myPushPlank.pull_plank_separate();
    //Can ext
    myHoldCans.releaseExternal();
    // Push plank
    myPushPlank.move_to_mm(-110);
    // Lift + cremaillere
    myPushPlank.pull_plank_release();
    //Lift up (for cremaillère)
    myLift.up_and_down(floorHeight+10);
    //Cremaillère back home
    myPushPlank.move_to_mm(0);

    myLift.up_and_down(secondStageHeight+5);
    delay(2000);
    myLift.up_and_down(secondStageHeight);
    delay(2000);
    myHoldCans.releaseCenter();
    Wire.write(CMD_BUILD_DONE);
    delay(5000);
    myLift.up_and_down(floorHeight);
}

void Teensy::test_lift(){
    myHoldCans.grabAll();
    delay(2000);
    myHoldCans.releaseCenter();
    myLift.up_and_down(thirdStageHeight);
}