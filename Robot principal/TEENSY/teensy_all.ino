#include <Arduino.h>

#include "hold_cans.h"
#include "lift.h"
#include "push_plank.h"
#include "infrared.h"
#include "get_input.h"

lift myLift;
hold_cans myHoldCans;
push_plank myPushPlank;

//Define I2C cmd
#define CMD_START_IR 0x01
#define CMD_GRAB 0x02
#define CMD_SEPARATE_STACK 0x03
#define CMD_GET_POSITION 0x04
#define CMD_GRAB_DONE 0x05

#define floorHeight 90
#define secondStageHeight 214 // in mm = 90 + 124
#define thirdStageHeight 338 // in mm = 90 + 124 + 124

volatile bool grabInProgress = false;
volatile int distance ; 

void build_second_stage(){
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


void receiveEvent(int bytes) {
    if (Wire.available()) {
        byte command = Wire.read();
        switch (command) {
            case CMD_START_IR:
                distance = give_distance();
                break;
            case CMD_GRAB:
                grabInProgress = true;
                myHoldCans.grabAll();
                delay(2000);
                blinkLED(3);
                myLift.up_and_down(110); //2cm 
                delay(2000);
                myPushPlank.pull_plank_grab();
                delay(100);
                grabInProgress = false ; 
                Wire.write(CMD_GRAB_DONE);
            break;
                break;
            case CMD_SEPARATE_STACK:
                //build_second_stage();
                break;
        }
    }
}

void requestEvent() {
    Wire.write(distance & 0xFF);
    Wire.write((distance >> 8) & 0xFF);
}

void setup() {

    Serial.begin(115200);

    myPushPlank.begin();
    myHoldCans.begin();
    myLift.begin();
    //init_input_rasp();

    myHoldCans.releaseAll();

    Wire.begin(0x08); // Adresse I2C de la Teensy
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
    delay(1000);
}

void loop() {
}

