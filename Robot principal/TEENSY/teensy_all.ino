#include "src/hold_cans/hold_cans.h"
#include "src/lift/lift.h"

lift myLift;
hold_cans myHoldCans;

#define floorHeight 0
#define secondStageHeight 150 // 124 mm
#define thirdStageHeight 200

void setup() {

    Serial.begin(115200);

    myLift.begin();
    myHoldCans.begin();

    
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
    myHoldCans.grabCenter();
    delay(1000);
    myLift.up_and_down(secondStageHeight);
    delay(6000);
    myHoldCans.releaseCenter();
    delay(2000);
    myLift.up_and_down(floorHeight);
    delay(6000);
    myHoldCans.grabAll();
    delay(2000);
    myHoldCans.releaseAll();
    delay(2000);

    /*Serial.println("Center...");
    myHoldCans.grabCenter();
    delay(3000);
    myHoldCans.releaseCenter();
    delay(3000);

    Serial.println("External...");
    myHoldCans.grabExternal();
    delay(3000);
    myHoldCans.releaseExternal();
    delay(3000);

    myHoldCans.grabAll();
    delay(3000);
    myHoldCans.releaseAll();
    delay(3000);*/
}