// #include <iostream>
// #include <thread>
// #include <unistd.h>
// #include <signal.h>
// #include <limits>
// #include <stdio.h>
// #include <mutex>

// #include "../include/Robot.h"
// #include "../include/struct.h"
// #include "../include/lidar.h"
// #include <csignal>

// // Variables globales pour le multithreading
// volatile bool running = true;
// // Mutex to protect the counter from race conditions
// std::mutex counterMutex;

// Robot robot;

// int counter = 1;
// // Function to handle SIGINT (Ctrl+C)
// void signalHandler(int signum) {
//     std::cout << "Interrupt signal (" << signum << ") received.\n";

//     // cleanup and close up stuff here  
//     // terminate program

//     running = false;
//     robot.stop();

//     exit(signum);
// }   

// // Function for thread that increments by 1
// void incrementByOne(int numIterations) {
//     for (int i = 0; i < numIterations; i++) {
//         // Lock the mutex before modifying the shared counter
//         counterMutex.lock();
//         counter += 1;
//         std::cout << "Thread 1: counter = " << counter << std::endl;
//         counterMutex.unlock();
        
//         // Small delay to make thread interleaving more visible
//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }
// }

// // Function for thread that increments by 10
// void incrementByTen(int numIterations) {
//     for (int i = 0; i < numIterations; i++) {
//         // Lock the mutex before modifying the shared counter
//         counterMutex.lock();
//         counter += 10;
//         std::cout << "Thread 2: counter = " << counter << std::endl;
//         counterMutex.unlock();
        
//         // Small delay to make thread interleaving more visible
//         std::this_thread::sleep_for(std::chrono::milliseconds(15));
//     }
// }


// int main() {

//     // Signal handler initialization
//     signal(SIGINT, signalHandler);

//     GAME *game = init_game();
    
//     running = true;

//     //Screen screen;

//     robot.loadNodes("src/Mobility/Localization/nodes.txt", game);
//     robot.loadEdges("src/Mobility/Localization/links.txt", game);
//     char choice;

//     std::cout << "Select an option:" << std::endl;
//     std::cout << "  a: boucles threads" << std::endl;
//     std::cout << "  b: main thread" << std::endl;
//     std::cout << "  c: path planning test" << std::endl;
//     std::cout << "  d: Middle level test" << std::endl;
//     std::cout << "  e: High level test" << std::endl;
//     std::cout << "  f: N/A" << std::endl;
//     std::cout << "  g: N/A" << std::endl;
//     std::cout << "  h: N/A" << std::endl;
//     std::cout << "  i: N/A" << std::endl;
//     std::cout << "  j: N/A" << std::endl;
//     std::cout << "  k: N/A" << std::endl;
//     std::cout << "  l: N/A" << std::endl;
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
//         case 'a': {
//             std::cout << "Running threads..." << std::endl;

//             break;
//         }

//         case 'b': {
//             std::cout << "Running main thread..." << std::endl;
            
//             std::cout << "Initial counter value: " << counter << std::endl;
    
//             // Create two threads
//             std::thread thread1(incrementByOne, 5);
//             std::thread thread2(incrementByTen, 5);
            
//             // Wait for threads to finish
//             thread1.join();
//             thread2.join();
            
//             std::cout << "Final counter value: " << counter << std::endl;
            
//             break;
//         }
        
//         case 'c': {
//             robot.aStar(0, 12, game);
//             robot.printPath();
//             break;
//         }

//         case 'd': {
//             std::cout << "Middle level test..." << std::endl;

//             robot.params = deplacement;
            
//             // Get target coordinates from user
//             std::cout << "Enter target X coordinate (meters): ";
//             std::cin >> robot.x_coord_target;
//             std::cout << "Enter target Y coordinate (meters): ";
//             std::cin >> robot.y_coord_target;
            

//             robot.middleLevelTest(game);
//             break;
//         }

//         case 'e': {
//             std::cout << "High level test..." << std::endl;
            
//             // Get target coordinates from user
//             double goal;
//             std::cout << "Enter goal node number: ";
//             std::cin >> goal;

//             unsigned long startloop;
//             unsigned long looptime;
//             int counterMid = 0;
//             int counterHigh = 100;

//             while (running) {
//                 startloop = micros();
        
//                 robot.updateOdometry(game);
                
//                 //std::cout << "X: " << sv.xCoord << ", Y: " << sv.yCoord << ", Theta: " << sv.theta *180/(M_PI)<< std::endl;
//                 // print speed
//                 if(counterHigh == 100){
//                     robot.highLevelController(goal, game);
//                     counterHigh = 0;
//                 }
//                 counterHigh++;

//                 if(counterMid == 10){
//                     robot.middleLevelController(game);
//                     counterMid = 0;
//                 }
//                 counterMid++;
                
//                 //fprintf(stderr, "middle ref speed left: %f, right: %f\n", robot.middle_ref_speed_left, robot.middle_ref_speed_right);
//                 robot.lowLevelController();

//                 looptime = micros() - startloop;
//                 if (looptime > robot.SAMPLING_TIME*1e6) {
//                     std::cout << "Loop time exceeded: " << looptime << std::endl;
//                 }
//                 usleep(robot.SAMPLING_TIME*1e6 - looptime);
//                 }
//             break;
//         }

//         case 'f': {
//             std::cout << "maneuvre test..." << std::endl;
            
//             double distance_manoeuvre;
//             // Get target coordinates from user
//             std::cout << "Enter manoeuvre distance (meters): ";
//             std::cin >> distance_manoeuvre;
            
//             robot.maneuver(distance_manoeuvre, game);
//             unsigned long startloop;
//             unsigned long looptime;
//             int counter = 0;
//             while (true) {
//                 startloop = micros();
        
//                 robot.updateOdometry(game);
                
//                 //std::cout << "X: " << sv.xCoord << ", Y: " << sv.yCoord << ", Theta: " << sv.theta *180/(M_PI)<< std::endl;
//                 // print speed
                
//                 if(counter == 10){
//                     robot.middleLevelController(game);
//                     counter = 0;
//                 }
//                 counter++;
                
//                 //fprintf(stderr, "middle ref speed left: %f, right: %f\n", robot.middle_ref_speed_left, robot.middle_ref_speed_right);
//                 robot.lowLevelController();
    
//                 looptime = micros() - startloop;
//                 if (looptime > robot.SAMPLING_TIME*1e6) {
//                     std::cout << "Loop time exceeded: " << looptime << std::endl;
//                 }
//                 usleep(robot.SAMPLING_TIME*1e6 - looptime);
//                 }
//             break;
//         }

//         case 'p': {
            
//             std::cout << "Seending information to teensy" << std::endl;
//             robot.teensy_init();
//             std::cout << "Initialisation OK" << std::endl;
//             robot.teensy_send_command(0x02);
//             std::cout << "C'est envoyé" << std::endl;
//             break ; 
            
//         }
//         case 'q': {
//             std::cout << "Seending information to teensy" << std::endl;
//             robot.teensy_init();
//             std::cout << "Initialisation OK" << std::endl;
//             robot.teensy_build();
//             break ; 
//         }

//         default:
//             std::cout << "Invalid option." << std::endl;
//             break;
//     }

//     // Arrêter le thread et nettoyer les ressources
//     running = false;
//     free_game(game);
    
//     std::cout << "Programme terminé avec succès" << std::endl;

//     return 0;
// }