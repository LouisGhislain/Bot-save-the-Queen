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
            case CMD_CLEAR : 
                myPushPlank.begin();
                myHoldCans.begin();
                myLift.begin();
            
                myHoldCans.releaseAll();
                myPushPlank.pull_plank_release();
                break;
            case CMD_START_IR:
                distance = give_distance();
                break;
            case CMD_GRAB:
                grabInProgress = true;
                myHoldCans.grabAll();
                delay(200);
                myLift.up_and_down(98); //2cm 
                delay(200);
                myPushPlank.pull_plank_grab();
                delay(100);
                grabInProgress = false ; 
                Wire.write(CMD_GRAB_DONE);
                break;
            case CMD_DROP_CANS_EXT : 
                myHoldCans.releaseExternal();
                break;
            case CMD_PUSH_PLANK :
                myPushPlank.move_to_mm(-110);
                break; 
            case CMD_CANS_UP :
                myPushPlank.pull_plank_release();
                delay(2000);
                myPushPlank.move_to_mm(0);
                delay(2000);
                myLift.up_and_down(220);
                delay(2000);
                Wire.write(CMD_CANS_UP_DONE);
                break;
            case CMD_DROP_SECOND_STAGE : 
                myLift.up_and_down(214);
                myHoldCans.releaseCenter();
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

    myHoldCans.releaseAll();

    Wire.begin(0x08); // Adresse I2C de la Teensy
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
    delay(1000);
}

void loop() {
}

