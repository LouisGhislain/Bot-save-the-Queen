// Teensy.h
#ifndef TEENSY_H
#define TEENSY_H

#include <Arduino.h>

#include "hold_cans.h"
#include "lift.h"
#include "push_plank.h"
#include "infrared.h"
#include "get_input.h"

class Teensy {

public :
    Teensy();
    void clear();
    void grab();
    void build();
    void secondStage();

private :
    lift myLift;
    hold_cans myHoldCans;
    push_plank myPushPlank;

    //Define I2C cmd
    #define CMD_START_IR 0x01
    #define CMD_GRAB 0x02
    #define CMD_GRAB_DONE 0x03
    #define CMD_DROP_CANS_EXT 0x04
    #define CMD_PUSH_PLANK 0x05
    #define CMD_CANS_UP 0x06
    #define CMD_CANS_UP_DONE 0x07
    #define CMD_DROP_SECOND_STAGE 0x08
    #define CMD_CLEAR 0x10

    #define floorHeight 90
    #define secondStageHeight 214 // in mm = 90 + 124
    #define thirdStageHeight 338 // in mm = 90 + 124 + 124

}




#endif // TEENSY_H