#include "../../include/Teensy.h"

void Teensy::separate_for_third_stage(){
        //Lift down
        myLift.up_and_down(floorHeight+5); 
        // Can ext
        myHoldCans.releaseExternal();
        // Push plank
        myPushPlank.move_to_mm(-110);
        Wire.write(CMD_RASP_BACKWARD);
        // Lift + cremaillere
        myPushPlank.pull_plank_release();
        delay(2000);
        //Lift up (for cremaillère)
        myLift.up_and_down(floorHeight+10);
        //Cremaillère back home
        myPushPlank.move_to_mm(0);
        delay(2000);
}

void Teensy::build_1_third_stage(){
        myLift.up_and_down(thirdStageHeight+10);
        Wire.write(CMD_RASP_FORWARD);
        //DELAY attendre que je sois au dessus du stack
        myHoldCans.releaseCenter();
        Wire.write(CMD_RASP_BACKWARD);
        delay(2000);
        myLift.up_and_down(floorHeight+10);
}

void Teensy::build_2_third_stage(){
        myHoldCans.grabExternal();
        myLift.up_and_down(thirdStageHeight+10);
        Wire.write(CMD_RASP_FORWARD);
        //DELAY attendre que je sois au dessus du stack
        myHoldCans.releaseExternal();
        Wire.write(CMD_RASP_BACKWARD);
        delay(2000);
        myLift.up_and_down(floorHeight+10);


}