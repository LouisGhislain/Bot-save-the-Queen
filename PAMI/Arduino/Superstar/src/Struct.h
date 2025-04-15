#ifndef STRUCT_H
#define STRUCT_H

// Enum pour les états du robot
typedef enum {
    WAIT,
    STRAIGHT1,
    TURN,
    STRAIGHT2,
    QUEUE,
    END
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