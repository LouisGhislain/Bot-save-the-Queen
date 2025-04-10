#include "../../include/Teensy.h"

void Teensy::build(){

    // Can ext
    myHoldCans.releaseExternal();
    // Push plank
    myPushPlank.move_to_mm(-110);

    // Lift + cremaillere
    myPushPlank.pull_plank_release();
    delay(2000);
    myPushPlank.move_to_mm(0);
    delay(2000);
    myLift.up_and_down(220);
    delay(2000);
    Wire.write(CMD_CANS_UP_DONE);

    // Release
    myLift.up_and_down(214);
    myHoldCans.releaseCenter();

}