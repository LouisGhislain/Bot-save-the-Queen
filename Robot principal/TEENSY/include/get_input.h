#ifndef GET_INPUT_H
#define GET_INPUT_H

#include <Arduino.h>

#define pinCommunicationRasp_1 5
#define pinCommunicationRasp_2 6
#define pinCommunicationRasp_3 7

void init_input_rasp();
int get_input_rasp(bool verbose);
int get_input_serial();


#endif // GET_INPUT_H