// #include <iostream>
// #include <thread>
// #include <unistd.h>
// #include <signal.h>
// #include <limits>
// #include <stdio.h>
// #include <mutex>
// #include <atomic>
// #include <chrono>
// #include <condition_variable>

// #include "../include/Robot.h"
// #include "../include/struct.h"
// #include <csignal>

// // Mutex to protect the counter from race conditions
// std::mutex data_mutex;
// // Flag to signal threads to terminate
// std::atomic<bool> running(false);

// // // Controller thread function that runs at specified interval
// // void loop_1ms(Robot robot, GAME *game){
// //     using namespace std::chrono;
    
// //     std::cout << "1ms loop started" << std::endl;
    
// //     while (!stop_threads) {
// //         auto start_time = steady_clock::now();
        
// //         // functions to be executed at the same iteration
// //         robot.updateOdometry(game);
// //         robot.lowLevelController();

// //         // Calculate how long to sleep to maintain desired frequency
// //         auto elapsed = duration_cast<milliseconds>(steady_clock::now() - start_time);
// //         auto sleep_time = milliseconds(1) - elapsed;
        
// //         if (sleep_time > milliseconds(0)) {
// //             std::this_thread::sleep_for(sleep_time);
// //         } else {
// //             // Controller took longer than interval - log warning
// //             std::cout << "Warning: 1ms loop exceeded time budget by "
// //                       << -sleep_time.count() << "ms" << std::endl;
// //         }
// //     }
// // }

// // void loop_10ms(Robot robot, GAME *game){
// //     using namespace std::chrono;
    
// //     std::cout << "10ms loop started" << std::endl;
    
// //     while (!stop_threads) {
// //         auto start_time = steady_clock::now();
        
// //         // functions to be executed at the same iteration
// //         robot.middleLevelController(game);

// //         // Calculate how long to sleep to maintain desired frequency
// //         auto elapsed = duration_cast<milliseconds>(steady_clock::now() - start_time);
// //         auto sleep_time = milliseconds(10) - elapsed;
        
// //         if (sleep_time > milliseconds(0)) {
// //             std::this_thread::sleep_for(sleep_time);
// //         } else {
// //             // Controller took longer than interval - log warning
// //             std::cout << "Warning: 10ms loop exceeded time budget by "
// //                       << -sleep_time.count() << "ms" << std::endl;
// //         }
// //     }
// // }

// // // void loop_100ms(Robot robot, GAME *game){
// // //     using namespace std::chrono;
    
// // //     std::cout << "100ms loop started" << std::endl;
    
// // //     while (!stop_threads) {
// // //         auto start_time = steady_clock::now();
        
// // //         // functions to be executed at the same iteration
// // //         FSM();

// // //         // Calculate how long to sleep to maintain desired frequency
// // //         auto elapsed = duration_cast<milliseconds>(steady_clock::now() - start_time);
// // //         auto sleep_time = milliseconds(100) - elapsed;
        
// // //         if (sleep_time > milliseconds(0)) {
// // //             std::this_thread::sleep_for(sleep_time);
// // //         } else {
// // //             // Controller took longer than interval - log warning
// // //             std::cout << "Warning: 100ms loop exceeded time budget by "
// // //                       << -sleep_time.count() << "ms" << std::endl;
// // //         }
// // //     }
// // // }

