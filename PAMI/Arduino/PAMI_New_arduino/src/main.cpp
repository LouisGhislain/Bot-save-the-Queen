#include <Arduino.h>
#include <Servo.h>
#include "Motor.h"
#include "Encoder.h"
#include "Tail.h"
#include "Microswitch.h"


volatile int leftTicks_data = 0;
volatile int rightTicks_data = 0;
const float TIME_INTERVAL = 0.0005; // 500µs en secondes
float Interval = 0;
unsigned long startTime;
int iteration = 0;



void setup() {
    Serial.begin(115200);
    Serial.println("Attente de 5 secondes...");
    delay(10000); // Attente avant le démarrage du test

    Pin_Motor_Initialization();
    
}

void loop() {
    set_motor(0, 0);
    delay(2000);
    set_motor(8, 8);
    delay(5000);


}


    
    
    // // Si le switch est pressé (état bas)
    // if (switchState == LOW) {
    //     // Stopper les moteurs
    //     analogWrite(ENA, 0);
    //     analogWrite(ENB, 0);
    //     Serial.println("Switch pressé, arrêt des moteurs.");
    //     while (true);
    // }
    
    // Attendre un petit moment avant de refaire la lecture

// void loop() {
//     unsigned long startTime = millis();
//     unsigned long lastMeasureTime = startTime;

//     while (millis() - startTime < 1500) { // 1 seconde
//         if (millis() - lastMeasureTime >= 1) { // Toutes les 500 µs
//             noInterrupts();
//             int leftTickCount = leftTicks;
//             int rightTickCount = rightTicks;
//             leftTicks = 0;
//             rightTicks = 0;
//             interrupts();

//             // Conversion en vitesse angulaire (rad/s)
//             float leftSpeedRad = (leftTickCount * 2.0 * PI) / (TICKS_PER_REV * (millis() - lastMeasureTime));
//             float rightSpeedRad = (rightTickCount * 2.0 * PI) / (TICKS_PER_REV * (millis() - lastMeasureTime));

//             Serial.print("Left: ");
//             Serial.print(leftSpeedRad);
//             Serial.print(" rad/s, Right: ");
//             // Serial.print(" ");
//             Serial.println(rightSpeedRad);
//             //Print le temps
//             // Serial.print("Temps: ");
//             // Serial.println(micros() - lastMeasureTime);
            
//             lastMeasureTime = millis();
//         }
//     }

//     analogWrite(ENA, 0);
//     analogWrite(ENB, 0);

//     Serial.println("Test terminé.");
//     while (true);
// }
