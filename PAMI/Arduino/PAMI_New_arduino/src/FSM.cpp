#include "FSM.h"
/*
bool isStopped = false;
PAMI pami ; 

void fsm(){
    double distance = pami.getSonarDistance();
    delay(60);
    if (distance < 8.0) {
        pami.pami_brake();
        isStopped = true;
        return;  // On ne fait rien d'autre si obstacle
    }
    else{
        isStopped = false;
    }

    if (!isStopped && distance!=0) {
        switch (pami.getState()){
            //case waiting_start:
            //    if(pami.isStartPressed()){
            //        pami.setState(moving);
            //    }
            //    break;

            case moving:
                pami.lowlevelcontrol(0.1,0.1);
                if(pami.isLeftPressed()){ //METTRE SWITCH DROIT ET "ARRIVE A DESTINATION"
                    pami.setState(dancing);
                }
                break;

            case dancing:
                pami.turnTail();
                break;
            
            default:
                break;
        }

    }
}
    
  // Vérifie la distance en permanence
//     double distance = Sonar_Get_Distance();
//     delay(60);
//     //Serial.print("Distance : ");
//     //Serial.print(distance, 2);  // 2 décimales
//     //Serial.println(" cm");


//       if (distance < 8.0) {
//           active_brake();
//           Serial.print("JE FREINE");
//           isStopped = true;
//           return;  // On ne fait rien d'autre si obstacle
//       } else {
//           isStopped = false;
//       }
  
//       // S'il n'y a pas d'obstacle, exécuter la séquence de mouvement sans delay bloquant
//       unsigned long currentTime = millis();
  
//       if (currentTime - previousStepTime > 20 && !isStopped && distance!=0) {
//           switch (motionStep) {
//               case 0:
//                   set_motor(4, 5);  // Avance
//                   previousStepTime = currentTime;
//                   motionStep++;
//                   break;
//               case 1:
//                   set_motor(3, 3);  // Tourne un peu
//                   previousStepTime = currentTime;
//                   motionStep++;
//                   break;
//               case 2:
//                   set_motor(4, 5);  // Reprend l'avance
//                   previousStepTime = currentTime;
//                   motionStep = 0;  // Reboucle
//                   break;
//           }
//       }
//   }
*/