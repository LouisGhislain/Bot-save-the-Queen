#include <iostream>
#include "Robot.h"
#include "Screen.h"
#include "SharedStruct.h"
#include <unistd.h> // For usleep function
#include <cmath>

int main() {
    Robot robot;
    //Screen screen;

    char choice;

    std::cout << "Select an option:" << std::endl;
    std::cout << "  b: Odometry (x,y,theta)" << std::endl;
    std::cout << "  c: Print robot speed (left motor)" << std::endl;
    std::cout << "  t: Open Loop test" << std::endl;
    std::cout << "  d: Distance test (meters)" << std::endl;
    std::cout << "  l: Lowlevel test" << std::endl;
    std::cout << "  f: Braking test" << std::endl;
    std::cout << "  u: BZZZ BZZZZ" << std::endl;
    std::cout << "  x: Teensy - cans" << std::endl;
    std::cout << "  y: Teensy - lift" << std::endl;
    std::cout << "  z: Teensy - lift+cans" << std::endl;
    std::cout << "  o: Test OLED" << std::endl;
    std::cout << "  m: Middle level" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    try {
        robot.start();  // This will initialize SPI and perform other setup tasks.
        robot.initCoords(GAME); // Initialize coordinates
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    switch(choice) {
        case 'b': {
            /*
            while (true) {
                robot.updateOdometry();
                std::cout << "X: " << robot.xCoord << ", Y: " << robot.yCoord << ", Theta: " << robot.theta *360/(2*M_PI)<< std::endl;
                usleep(100000); // Sleep for 0.1 seconds
            }
            */
            unsigned long startloop;
            unsigned long looptime;
        
            while (true) {
                startloop = micros();
        
                robot.updateOdometry();
                std::cout << "X: " << sv.xCoord << ", Y: " << sv.yCoord << ", Theta: " << sv.theta *180/(M_PI)<< std::endl;
                
                looptime = micros() - startloop;
                if (looptime > robot.SAMPLING_TIME*1e6) {
                    std::cout << "Loop time exceeded: " << looptime << std::endl;
                }
                usleep(robot.SAMPLING_TIME*1e6 - looptime);
                }

            break;
        }
        case 'c': {
            // NOTE: Since Robot does not provide a direct getSpeed() function,
            // one option is to use one motor’s speed as a proxy.
            // Alternatively, you might add a public method to Robot to get speed.
            std::cout << "Retrieving left motor speed..." << std::endl;
            // Assuming we would like to access the left motor's speed, we could add a public method to Robot:
            // double speed = robot.getLeftMotorSpeed();
            // For now, we will print a message.
            std::cout << "Robot speed functionality is not yet implemented." << std::endl;
            break;
        }
        case 't':{
            std::cout << "Acquiring open loop data..." << std::endl;
            robot.openLoopData();
            break;
        }
        case 'd':{
            std::cout << "Printing distance..." << std::endl;
            robot.printDistance();
            break;
        }
        case 'l':{
            std::cout << "Testing low level..." << std::endl;
            robot.lowLevelTest();
            break;
        }
        case 'f':{
            std::cout << "Braking test..." << std::endl;
            robot.stop();
            break;
        }
        case 'u':{
            std::cout << "BZZZ BZZZZZ" << std::endl;
            robot.buzzBuzzer();
            break;
        }
        case 'x' :{
            std::cout << "Sending '01' on Teensy board" << std::endl;
            robot.teensy_cans();
            break;
        }
        case 'y' :{
            std::cout << "Sending '10' on Teensy board" << std::endl;
            robot.teensy_lift();
            break;
        }
        case 'z' :{
            std::cout << "Sending '11' on Teensy board" << std::endl;
            robot.teensy_cans_lift();
            break;
        }

        case 'w': {
            break;
        }

        case 'm': {
            std::cout << "Middle level test..." << std::endl;
            
            // Get target coordinates from user
            float targetX, targetY;
            std::cout << "Enter target X coordinate (meters): ";
            std::cin >> targetX;
            std::cout << "Enter target Y coordinate (meters): ";
            std::cin >> targetY;

            unsigned long startloop;
            unsigned long looptime;
            int counter = 0;
            while (true) {
                startloop = micros();
        
                robot.updateOdometry();
                
                //std::cout << "X: " << sv.xCoord << ", Y: " << sv.yCoord << ", Theta: " << sv.theta *180/(M_PI)<< std::endl;
                // print speed
                std::cout << "speed vref = " << robot.vref << std::endl;
                
                if(counter == 10){
                    robot.middleLevelController(targetX, targetY, 0, deplacement, GAME);
                    counter = 0;
                }
                counter++;

                looptime = micros() - startloop;
                if (looptime > robot.SAMPLING_TIME*1e6) {
                    std::cout << "Loop time exceeded: " << looptime << std::endl;
                }
                usleep(robot.SAMPLING_TIME*1e6 - looptime);
                }
            break;
        }

        /*case 'o': {
            screen.init();
            std::string message;
            std::cout << "Entrez le message à afficher : ";
            std::cin.ignore(); // Évite un problème de buffer
            std::getline(std::cin, message);
            screen.clear();
            screen.displayText(message);
            break;
        }*/
        
        default:
            std::cout << "Invalid option." << std::endl;
            break;
    }

    return 0;
}
