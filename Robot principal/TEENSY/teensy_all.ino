#include <Arduino.h>

#include "hold_cans.h"
#include "lift.h"
#include "push_plank.h"
#include "infrared.h"
#include "get_input.h"

lift myLift;
hold_cans myHoldCans;
push_plank myPushPlank;


#define floorHeight 90
#define secondStageHeight 214 // in mm = 90 + 124
#define thirdStageHeight 338 // in mm = 90 + 124 + 124

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
    delay(1000);
}

void setup() {

    Serial.begin(115200);

    myPushPlank.begin();
    delay(2000);
    myHoldCans.begin();
    delay(100);
    myLift.begin();
    init_input_rasp();

    myHoldCans.releaseAll();
    delay(1000);
}

int angle = 0;

void loop() {

    //==================================================================================================
    // Temporary code
    //==================================================================================================

    build_second_stage();
    delay(100);

    //==================================================================================================
    // end of Temporary code
    //==================================================================================================

    int rasp_input = get_input_rasp(0); // verbose = 0 (no prints) 
    rasp_input = 0; // for testion, input imposed to 0

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
}

