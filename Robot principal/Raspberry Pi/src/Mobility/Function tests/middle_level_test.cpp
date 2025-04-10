#include "../../../include/Robot.h"
#include <fstream>



void Robot::middleLevelTest(double targetX, double targetY, void *sqid) {
    GAME * game = (GAME *)sqid;
    Queen * queen = game->queen;

    // Open file
    std::ofstream file;
    file.open("middle_level_test.txt", std::ios::out);
    
    if (!file) {
        std::cout << "Erreur ouverture fichier" << std::endl;
        return;
    }
    file << "0, " << middle_ref_speed_left << ", "<< middle_ref_speed_right << ", "<< leftMotor.getSpeed() << ", " << rightMotor.getSpeed() << ", " << queen->cart_pos->x << ", " << queen->cart_pos->y << "\n";

    unsigned long startTime = micros(); // Current time in µs
    unsigned long duration = 20*1000000; // To seconds;
    unsigned long currentTime;
    unsigned long startloop;
    unsigned long looptime;

    int counter = 0;

    fprintf(stderr, "START Middle level test...\n");

    while (micros() - startTime < duration) {
        startloop = micros();

        currentTime = micros() - startTime;
        
        updateOdometry(game);
        
        //std::cout << "X: " << sv.xCoord << ", Y: " << sv.yCoord << ", Theta: " << sv.theta *180/(M_PI)<< std::endl;
        // print speed
        
        if(counter == 10){
            middleLevelController(targetX, targetY, 0, manoeuvre, game);
            counter = 0;

        }
        counter++;
        
        lowLevelController(middle_ref_speed_left, middle_ref_speed_right);

        file << currentTime << ", " << middle_ref_speed_left << ", "<< middle_ref_speed_right << ", "
                << leftMotor.getSpeed() << ", " 
                << rightMotor.getSpeed() << ", "
                << queen->cart_pos->x << ", " 
                << queen->cart_pos->y << "\n";
        
        looptime = micros() - startloop;
        if (looptime > SAMPLING_TIME*1e6) {
            std::cout << "Loop time exceeded: " << looptime << std::endl;
        }
        // else{
        //     //std::cout << "Loop time okay: " << looptime << std::endl;
        // }
        
        usleep(SAMPLING_TIME*1e6 - looptime);
    }

    stop();
    file.close();
    std::cout << "Test completed" << std::endl;
}