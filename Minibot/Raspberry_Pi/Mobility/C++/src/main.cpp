#include <iostream>
#include "Robot.h"
#include <thread>

int main() {
    Robot robot;
    char choice;

    std::cout << "Select an option:" << std::endl;
    std::cout << "  a: Run routine" << std::endl;
    std::cout << "  b: Print robot position (angle)" << std::endl;
    std::cout << "  c: Print robot speed (left motor)" << std::endl;
    std::cout << "  t: Motor test" << std::endl;
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
            std::cout << "Testing low level controller..." << std::endl;
            robot.testMotors();
            break;
        }
        default:
            std::cout << "Invalid option." << std::endl;
            break;
    }

    return 0;
}
