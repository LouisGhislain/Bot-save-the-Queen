#ifndef STRUCT_H
#define STRUCT_H

// Enum pour les états du robot
typedef enum {
    STRAIGHT1,
    TURN,
    STRAIGHT2,
    QUEUE
} EtatRobot;

// Structure représentant l'état du robot
typedef struct {
    EtatRobot state;
    unsigned long startTime;
    bool waitDone;
    bool firstPathDone;
    bool turnDone;
    bool secondPathDone;
    bool switchActivated;
    bool EndDone;
} Robot;


#endif