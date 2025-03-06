
// 4 servo moteur dans cette config en regardant de dos le hold cans
//
// 4 () == 3 (M) ==== 2 () == 1 (M)

#include "servo_class_v1.h"
#include <PWMServo.h>


#define SERVO1_PIN 28 // Pin for the servo motor
#define SERVO2_PIN 29  
#define SERVO3_PIN 33  
#define SERVO4_PIN 36  

#define max_angle 60

ServoMotor servo1(SERVO1_PIN, max_angle+12, 12);  // Create a servo motor object: pin, grabAngle, releaseAngle
ServoMotor servo2(SERVO2_PIN, 0, max_angle); 
ServoMotor servo3(SERVO3_PIN, max_angle, 0);
ServoMotor servo4(SERVO4_PIN, 10, max_angle+10);

PWMServo servo11;
PWMServo servo12;
PWMServo servo13;
PWMServo servo14;

////////////////////////////////////////
// functions
////////////////////////////////////////

void grab_2_center(){
    servo2.grab();
    servo3.grab();
}

void release_2_center(){
    servo2.release();
    servo3.release();
}

void grab_2_external(){
    servo1.grab();
    servo4.grab();
}

void release_2_external(){
    servo1.release();
    servo4.release();
}

void release_4(){
    /*servo1.release();
    servo2.release();
    servo3.release();
    servo4.release();*/
    release_2_center();
    release_2_external();
}

void grab_4(){
    /*servo1.grab();
    servo2.grab();
    servo3.grab();
    servo4.grab();*/
    grab_2_center();
    grab_2_external();
}


////////////////////////////////////////
// code
////////////////////////////////////////

void setup() {
    servo11.attach(SERVO1_PIN);
    servo12.attach(SERVO2_PIN);
    servo13.attach(SERVO3_PIN);
    servo14.attach(SERVO4_PIN);

    release_4();
    delay(1000);

    Serial.begin(9600);
}

void loop() {
    grab_2_center();
    delay(3000);
    release_2_center();
    delay(3000);

    grab_4();
    delay(3000);
    release_4();
    delay(3000);
}
