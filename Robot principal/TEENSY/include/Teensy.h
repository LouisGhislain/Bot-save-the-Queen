// Teensy.h
#ifndef TEENSY_H
#define TEENSY_H

#include <Arduino.h>
#include <Wire.h>
#include "hold_cans.h"
#include "lift.h"
#include "push_plank.h"
#include "infrared.h"
#include "get_input.h"

class Teensy {

public :
    Teensy();

    // Actions
    void clear();
    void grab();
    void build();
    void separate_for_third_stage();
    void build_1_third_stage();
    void build_2_third_stage();
    void secondStage(); //DEMO RBODAY
    void test_lift();

private :
    lift myLift;
    hold_cans myHoldCans;
    push_plank myPushPlank;

    //Define I2C cmd
    #define CMD_START_IR 0x01
    #define CMD_GRAB 0x02
    #define CMD_GRAB_DONE 0x03
    #define CMD_BUILD 0x04
    #define CMD_RASP_BACKWARD 0x05
    #define CMD_RASP_FORWARD 0x06
    #define CMD_RASP_FORWARD_DONE 0x07
    #define CMD_BUILD_DONE 0x08
    #define CMD_CLEAR 0x10
    #define CMD_TEST 0x11

    #define floorHeight 90
    #define secondStageHeight 214 // in mm = 90 + 124
    #define thirdStageHeight 338 // in mm = 90 + 124 + 124
};

#endif // TEENSY_H