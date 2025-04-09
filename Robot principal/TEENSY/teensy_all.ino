#include <Arduino.h>

volatile bool grabInProgress = false;
volatile int distance ; 

void receiveEvent(int bytes) {
    if (Wire.available()) {
        byte command = Wire.read();
        switch (command) {

            case CMD_CLEAR : 
                teensy.clear();
                break;

            case CMD_START_IR:
                distance = give_distance();
                break;

            case CMD_GRAB:
                teensy.grab();
                break;

            case CMD_BUILD :
                teensy.build();
                break;
        }
    }
}

void requestEvent() {
    Wire.write(distance & 0xFF);
    Wire.write((distance >> 8) & 0xFF);
}

void setup() {

    // Communication I2C initialization
    Serial.begin(115200);
    Wire.begin(0x08); // Adresse I2C de la Teensy

    // Teensy class initialization
    Teensy teensy;
    teensy.clear();

    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
    delay(1000);
}

void loop() {
}

