#include <Arduino.h>

#include "hold_cans.h"
#include "lift.h"
#include "push_plank.h"
#include "infrared.h"

lift myLift;
hold_cans myHoldCans;
push_plank myPushPlank;


#define floorHeight 0
#define secondStageHeight 124 // 124 mm
#define thirdStageHeight 248 // 248 mm

#define pinCommunicationRasp_1 5
#define pinCommunicationRasp_2 6
#define pinCommunicationRasp_3 7

void setup() {

    Serial.begin(115200);

    pinMode(pinCommunicationRasp_1, INPUT);
    pinMode(pinCommunicationRasp_2, INPUT);
    pinMode(pinCommunicationRasp_3, INPUT);


    myLift.begin();
    myHoldCans.begin();
    myPushPlank.begin();

    
    myHoldCans.releaseAll();
    delay(1000);
}

void loop() {

    /*int number;
    if (Serial.available() > 0) {
        number = Serial.parseInt();  // Read full integer input
    
        myHoldCans.grabNumber(number);
        delay(2000);
        myHoldCans.releaseNumber(number);
        delay(2000);
    }*/

    //==================================================================================================
    // Temporary code
    //==================================================================================================

    /*myLift.up_and_down(secondStageHeight);
    delay(1000);
    myLift.up_and_down(thirdStageHeight);
    delay(1000);
    myLift.up_and_down(floorHeight);
    delay(1000);*/

    //==================================================================================================
    // end of Temporary code
    //==================================================================================================


    int rasp_input_1 = digitalRead(pinCommunicationRasp_1);
    int rasp_input_2 = digitalRead(pinCommunicationRasp_2);
    int rasp_input_3 = digitalRead(pinCommunicationRasp_3);
    int rasp_input = (rasp_input_1 << 2) | (rasp_input_2 << 1) | (rasp_input_3);

    //Serial.println(rasp_input_1);
    //Serial.println(rasp_input_2);
    if(rasp_input_1 == HIGH){
        Serial.println(rasp_input_1);
        Serial.println(rasp_input);
        delay(1000);
    }
    if(rasp_input_2 == HIGH){
        Serial.println(rasp_input_2);
        Serial.println(rasp_input);
        delay(1000);
    }
    if(rasp_input_3 == HIGH){
        Serial.println(rasp_input_3);
        Serial.println(rasp_input);
        delay(1000);
    }
    // create a binary number of the three input :

    switch (rasp_input){ // until 8
        case 0:
            /* code */
            break;
        case 1:
            /* code */
            break;
        case 2:
            myHoldCans.grabCenter();
            delay(5000);
            myHoldCans.releaseCenter();
            delay(1000);

            myHoldCans.grabAll();
            delay(5000);
            myHoldCans.releaseAll();
            delay(1000);
            break;
        case 3:

            break;
        case 4:
            myLift.up_and_down(secondStageHeight);
            delay(1000);
            myLift.up_and_down(thirdStageHeight);
            delay(1000);
            myLift.up_and_down(floorHeight);
            delay(1000);
            break;
        case 5:
            /* code */
            break;
        case 6:
            myHoldCans.releaseAll();
            delay(2000);
            myHoldCans.grabCenter();
            delay(500);
            myLift.up_and_down(secondStageHeight);
            delay(3000);
            myHoldCans.releaseCenter();
            delay(1000);
            myLift.up_and_down(floorHeight);
            break;
        case 7:
            /* code */
            break;
        default:
            break;
        }
    myLift.up_and_down(124);
}

void build_second_stage(){
    myHoldCans.releaseExternal();
    myHoldCans.grabCenter();
    delay(500);
    myPushPlank.routine_separation_stack();
    delay(1000);
    myLift.up_and_down(secondStageHeight);
    delay(1000);
    myHoldCans.releaseCenter();
    delay(1000);
    myLift.up_and_down(floorHeight);
}