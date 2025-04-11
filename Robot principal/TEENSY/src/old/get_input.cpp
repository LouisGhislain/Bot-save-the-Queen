#include "get_input.h"

void init_input_rasp() {
    pinMode(pinCommunicationRasp_1, INPUT);
    pinMode(pinCommunicationRasp_2, INPUT);
    pinMode(pinCommunicationRasp_3, INPUT);
}

int get_input_rasp(bool verbose) {
    int rasp_input_1 = digitalRead(pinCommunicationRasp_1);
    int rasp_input_2 = digitalRead(pinCommunicationRasp_2);
    int rasp_input_3 = digitalRead(pinCommunicationRasp_3);
    int rasp_input = (rasp_input_1 << 2) | (rasp_input_2 << 1) | (rasp_input_3);

    if(verbose == 1){
        if(rasp_input_1 == HIGH){
            Serial.println(rasp_input_1);
            Serial.println(rasp_input);
            delay(100);
        }
        if(rasp_input_2 == HIGH){
            Serial.println(rasp_input_2);
            Serial.println(rasp_input);
            delay(100);
        }
        if(rasp_input_3 == HIGH){
            Serial.println(rasp_input_3);
            Serial.println(rasp_input);
            delay(100);
        }
    }

    return rasp_input;
}

int get_input_serial() {
    int number = 0;
    if (Serial.available() > 0) {
        number = Serial.parseInt();  // Read full integer input
    }
    return number;
}