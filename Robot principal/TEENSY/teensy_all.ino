#include "src/hold_cans/hold_cans.h"
#include "src/lift/lift.h"

lift myLift;
hold_cans myHoldCans;

#define floorHeight 0
#define secondStageHeight 124
#define thirdStageHeight 200

void setup() {

    Serial.begin(9600);

    myLift.begin();
    myHoldCans.begin();
}

void loop() {

    /*int angle = 0;
    int val = 0;
    if (Serial.available() > 0) {
        angle = Serial.parseInt();  // Read full integer input
    
        if(angle > 0){
          val = map(angle, 0, 270, 6, 32);  // Ensure valid servo range
    
          Serial.print("Angle received: ");
          Serial.print(angle);
          Serial.print(" - PWM value: ");
          Serial.println(val);
          Serial.println(" ");
    
          analogWrite(33, val);
          analogWrite(28, val);  // Move servo to position
    
          delay(100);
        }
    }*/
    myLift.up_and_down(floorHeight);
    delay(1000);
    myLift.up_and_down(secondStageHeight);
    delay(1000);
    myLift.up_and_down(thirdStageHeight);
    delay(1000);


    /*Serial.println("Center...");
    myHoldCans.grabCenter();
    delay(3000);
    myHoldCans.releaseCenter();
    delay(3000);

    Serial.println("All...");
    myHoldCans.grabAll();
    delay(3000);
    myHoldCans.releaseAll();
    delay(3000);

    Serial.println("Finished");*/
}