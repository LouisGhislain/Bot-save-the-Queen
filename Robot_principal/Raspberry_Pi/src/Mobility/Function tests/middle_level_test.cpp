#include "../../../include/Robot.h"
#include <fstream>



void Robot::middleLevelTest(void *game) {
    GAME * mygame = (GAME *)game;
    Queen * myqueen = mygame->queen;

    

    // Open file
    std::ofstream file;
    file.open("middle_level_test.txt", std::ios::out);
    
    if (!file) {
        std::cout << "Erreur ouverture fichier" << std::endl;
        return;
    }
    file << "0, " << ref_speed_left << ", "<< ref_speed_right << ", "<< leftMotor.getSpeed() << ", " << rightMotor.getSpeed() << ", " << myqueen->cart_pos->x << ", " << myqueen->cart_pos->y << "\n";

    unsigned long startTime = micros(); // Current time in µs
    unsigned long duration = 10*1000000; // To seconds;
    unsigned long currentTime;
    unsigned long startloop;
    unsigned long looptime;

    int counter = 0;

    fprintf(stderr, "START Middle level test...\n");

    while (micros() - startTime < duration) {
        startloop = micros();

        currentTime = micros() - startTime;
        
        updateOdometry(mygame);
        
        //std::cout << "X: " << sv.xCoord << ", Y: " << sv.yCoord << ", Theta: " << sv.theta *180/(M_PI)<< std::endl;
        // print speed
        
        if(counter == 10){
            middleLevelController(mygame);
            counter = 0;

        }
        counter++;
        
        lowLevelController();

        file << currentTime << ", " << ref_speed_left << ", "<< ref_speed_right << ", "
                << leftMotor.getSpeed() << ", " 
                << rightMotor.getSpeed() << ", "
                << myqueen->cart_pos->x << ", " 
                << myqueen->cart_pos->y << "\n";
        
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