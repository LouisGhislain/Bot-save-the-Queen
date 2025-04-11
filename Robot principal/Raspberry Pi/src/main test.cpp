// #include <iostream>
// #include <pthread.h>
// #include <unistd.h>
// #include <signal.h>
// #include <limits>
// #include <stdio.h>
// #include "../include/Robot.h"
// #include "../include/struct.h"
// #include "../include/lidar.h"

// // Variables globales pour le multithreading
// volatile bool running = true;
// pthread_mutex_t data_mutex;
// pthread_t lidar_thread;




// int main() {
//     Robot robot;

//     GAME *game = init_game(); 
//     //Screen screen;

//     char choice;

//     std::cout << "Select an option:" << std::endl;
//     std::cout << "  b: Odometry (x,y,theta)" << std::endl;
//     std::cout << "  c: Print robot speed (left motor)" << std::endl;
//     std::cout << "  t: Open Loop test" << std::endl;
//     std::cout << "  d: Distance test (meters)" << std::endl;
//     std::cout << "  l: Lowlevel test" << std::endl;
//     std::cout << "  f: Braking test" << std::endl;
//     std::cout << "  u: BZZZ BZZZZ" << std::endl;
//     std::cout << "  x: Teensy - cans" << std::endl;
//     std::cout << "  y: Teensy - lift" << std::endl;
//     std::cout << "  z: Teensy - lift+cans" << std::endl;
//     std::cout << "  o: Test OLED" << std::endl;
//     std::cout << "  m: Middle level" << std::endl;
//     std::cout << "  w: Middle level test" << std::endl;
//     std::cout << "  a: low level" << std::endl;
//     std::cout << "Enter your choice: ";
//     std::cin >> choice;

//     try {
//         robot.start();  // This will initialize SPI and perform other setup tasks.
//         robot.initCoords(game); // Initialize coordinates
//     } catch (const std::exception& e) {
//         std::cerr << e.what() << std::endl;
//         return 1;
//     }

//     switch(choice) {
//         case 'b': {
//             /*
//             while (true) {
//                 robot.updateOdometry();
//                 std::cout << "X: " << robot.xCoord << ", Y: " << robot.yCoord << ", Theta: " << robot.theta *360/(2*M_PI)<< std::endl;
//                 usleep(100000); // Sleep for 0.1 seconds
//             }
//             */
//             unsigned long startloop;
//             unsigned long looptime;
        
//             while (true) {
//                 startloop = micros();
        
//                 robot.updateOdometry(game);
//                 std::cout << "X: " << game->queen->cart_pos->x << ", Y: " << game->queen->cart_pos->y << ", Theta: " << game->queen->angle *180/(M_PI)<< std::endl;
                
//                 looptime = micros() - startloop;
//                 if (looptime > robot.SAMPLING_TIME*1e6) {
//                     std::cout << "Loop time exceeded: " << looptime << std::endl;
//                 }
//                 usleep(robot.SAMPLING_TIME*1e6 - looptime);
//                 }

//             break;
//         }
//         case 'c': {
//             // NOTE: Since Robot does not provide a direct getSpeed() function,
//             // one option is to use one motor’s speed as a proxy.
//             // Alternatively, you might add a public method to Robot to get speed.
//             std::cout << "Retrieving left motor speed..." << std::endl;
//             // Assuming we would like to access the left motor's speed, we could add a public method to Robot:
//             // double speed = robot.getLeftMotorSpeed();
//             // For now, we will print a message.
//             std::cout << "Robot speed functionality is not yet implemented." << std::endl;
//             break;
//         }
//         case 't':{
//             std::cout << "Acquiring open loop data..." << std::endl;
//             robot.openLoopData();
//             break;
//         }
//         case 'd':{
//             std::cout << "Printing distance..." << std::endl;
//             robot.printDistance();
//             break;
//         }
//         case 'l':{
//             std::cout << "Testing low level..." << std::endl;
//             robot.lowLevelTest();
//             break;
//         }
//         case 'f':{
//             std::cout << "Braking test..." << std::endl;
//             robot.stop();
//             break;
//         }
//         case 'u':{
//             std::cout << "BZZZ BZZZZZ" << std::endl;
//             robot.buzzBuzzer();
//             break;
//         }
        
//         case 'w': {
//             std::cout << "Middle level test..." << std::endl;
            
//             // Get target coordinates from user
//             std::cout << "Enter target X coordinate (meters): ";
//             std::cin >> x_coord_target;
//             std::cout << "Enter target Y coordinate (meters): ";
//             std::cin >> y_coord_target;
            

//             robot.middleLevelTest(targetX, targetY, game);
//             break;
//         }

        // case 'm': {
        //     std::cout << "Middle level test..." << std::endl;
            
        //     // Get target coordinates from user
        //     double targetX, targetY;
        //     std::cout << "Enter target X coordinate (meters): ";
        //     std::cin >> targetX;
        //     std::cout << "Enter target Y coordinate (meters): ";
        //     std::cin >> targetY;

        //     unsigned long startloop;
        //     unsigned long looptime;
        //     int counter = 0;
        //     while (true) {
        //         startloop = micros();
        
        //         robot.updateOdometry(game);
                
        //         //std::cout << "X: " << sv.xCoord << ", Y: " << sv.yCoord << ", Theta: " << sv.theta *180/(M_PI)<< std::endl;
        //         // print speed
                
        //         if(counter == 10){
        //             robot.middleLevelController(targetX, targetY, 0, deplacement, game);
        //             counter = 0;
        //         }
        //         counter++;
                
        //         //fprintf(stderr, "middle ref speed left: %f, right: %f\n", robot.middle_ref_speed_left, robot.middle_ref_speed_right);
        //         robot.lowLevelController(robot.middle_ref_speed_left, robot.middle_ref_speed_right);

        //         looptime = micros() - startloop;
        //         if (looptime > robot.SAMPLING_TIME*1e6) {
        //             std::cout << "Loop time exceeded: " << looptime << std::endl;
        //         }
        //         usleep(robot.SAMPLING_TIME*1e6 - looptime);
        //         }
        //     break;
//         }

//         case 'a': {
//             double speed;
//             std::cout << "Enter speed : ";
//             std::cin >> speed;

//             std::cout << "Low level ..." << std::endl;
//             while (true) {
//                 robot.lowLevelController(speed, speed);
//                 usleep(1000); // Sleep for 0.001 seconds
//             }
//             break;
//         }

//         /*case 'o': {
//             screen.init();
//             std::string message;
//             std::cout << "Entrez le message à afficher : ";
//             std::cin.ignore(); // Évite un problème de buffer
//             std::getline(std::cin, message);
//             screen.clear();
//             screen.displayText(message);
//             break;
//         }*/
        
//         default:
//             std::cout << "Invalid option." << std::endl;
//             break;
//     }

//     return 0;
// }