#include <iostream>
#include "Robot.h"

int main() {
    Robot robot;

    char choice;

    std::cout << "Select an option:" << std::endl;
    std::cout << "  a: Run routine" << std::endl;
    std::cout << "  b: Print robot position (angle)" << std::endl;
    std::cout << "  c: Print robot speed (left motor)" << std::endl;
    std::cout << "  t: Motor test" << std::endl;
    std::cout << "  d: Distance test" << std::endl;
    std::cout << "  l: Lowlevel test" << std::endl;
    std::cout << "  f: Braking test" << std::endl;
    std::cout << "  u: BZZZ BZZZZ" << std::endl;
    std::cout << "  x: Teensy - cans" << std::endl;
    std::cout << "  y: Teensy - lift" << std::endl;
    std::cout << "  z: Teensy - lift+cans" << std::endl;
    std::cout << "  o: Test OLED" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    try {
        robot.start();  // This will initialize SPI and perform other setup tasks.
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    switch(choice) {
        case 'a':
            std::cout << "Launching routine..." << std::endl;
            robot.routine();
            break;
        
        case 'b': {
            double angle = robot.getAngle();
            std::cout << "Robot angle (as proxy for position): " << angle << " degrees" << std::endl;
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

        /*case 'o': {
            robot.screen_init();
            std::string message;
            std::cout << "Entrez le message à afficher : ";
            std::cin.ignore(); // Évite un problème de buffer
            std::getline(std::cin, message);
            robot.screen_clear();
            robot.screen_displayText(message);
            break;
        }*/
        
        default:
            std::cout << "Invalid option." << std::endl;
            break;
    }

    return 0;
}
