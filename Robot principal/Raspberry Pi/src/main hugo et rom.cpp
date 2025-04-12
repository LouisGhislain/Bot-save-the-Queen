// #include <iostream>
// #include <pthread.h>
// #include <unistd.h>
// // #include <signal.h>
// #include <limits>
// #include "Robot.h"
// #include "../include/struct.h"
// #include "../include/lidar.h"

// // Variables globales pour le multithreading
// volatile bool running = true;
// pthread_mutex_t data_mutex;
// pthread_t lidar_thread;

// // Gestionnaire de signal pour terminer proprement le programme
// void sigint_handler(int sig) {
//     running = false;
//     printf("Arrêt en cours...\n");
// }

// // // Fonction thread pour la récupération des données LIDAR
// // void* lidar_thread_func(void* game_void) {
// //     GAME* game = (GAME*)game_void;
    
// //     while (running) {
// //         // Verrouiller le mutex avant d'accéder aux données partagées
// //         pthread_mutex_lock(&data_mutex);
        
// //         // Appeler la fonction de récupération des données LIDAR
// //         fetchLidarData(game);
        
// //         // Déverrouiller le mutex après la mise à jour des données partagées
// //         pthread_mutex_unlock(&data_mutex);
        
// //         // Court délai pour éviter une utilisation excessive du CPU
// //         usleep(10000); // 10ms de délai
// //     }
    
// //     return NULL;
// // }

// int main() {
//     // Initialiser le gestionnaire de signal
//     signal(SIGINT, sigint_handler);
    
//     // Initialiser le mutex
//     if (pthread_mutex_init(&data_mutex, NULL) != 0) {
//         std::cerr << "Échec de l'initialisation du mutex" << std::endl;
//         return 1;
//     }

//     // Initialiser le robot et le jeu
//     Robot robot;
//     GAME *game = init_game();

// //     // Initialiser et démarrer le LIDAR
// //     init_connectLidar();
    
// //     // Créer le thread LIDAR
// //     if (pthread_create(&lidar_thread, NULL, lidar_thread_func, game) != 0) {
// //         std::cerr << "Échec de la création du thread" << std::endl;
// //         free_game(game);
// //         pthread_mutex_destroy(&data_mutex);
// //         return 1;
// //     }
    
// //     std::cout << "LIDAR démarré avec succès en arrière-plan (multithreading)" << std::endl;

//     char choice;

//     std::cout << "Sélectionnez une option:" << std::endl;
//     std::cout << "  b: Odométrie (x,y,theta)" << std::endl;
//     std::cout << "  c: Afficher la vitesse du robot (moteur gauche)" << std::endl;
//     std::cout << "  t: Test en boucle ouverte" << std::endl;
//     std::cout << "  d: Test de distance (mètres)" << std::endl;
//     std::cout << "  l: Test de bas niveau" << std::endl;
//     std::cout << "  f: Test de freinage" << std::endl;
//     std::cout << "  u: BZZZ BZZZZ" << std::endl;
//     std::cout << "  o: Test OLED" << std::endl;
//     std::cout << "  m: Niveau moyen" << std::endl;
//     std::cout << "  p: RESET teensy" << std::endl;
//     std::cout << "  q: SEPARATE1 teensy" << std::endl;
//     std::cout << "  s: Afficher la position Sauron du LIDAR" << std::endl;
//     std::cout << "Entrez votre choix: ";
//     std::cin >> choice;

//     try {
//         robot.start();  // Initialiser SPI et effectuer d'autres tâches de configuration
//         robot.initCoords(game); // Initialiser les coordonnées
//     } catch (const std::exception& e) {
//         std::cerr << e.what() << std::endl;
//         running = false;
//         pthread_join(lidar_thread, NULL);
//         pthread_mutex_destroy(&data_mutex);
//         free_game(game);
//         return 1;
//     }

//     switch(choice) {
//         case 'b': {
//             unsigned long startloop;
//             unsigned long looptime;
        
//             while (running) {
//                 startloop = micros();
                
//                 // Verrouiller le mutex avant d'accéder aux données
//                 pthread_mutex_lock(&data_mutex);
                
//                 robot.updateOdometry(game);
//                 std::cout << "X: " << game->queen->cart_pos->x << ", Y: " << game->queen->cart_pos->y 
//                           << ", Theta: " << game->queen->angle *180/(M_PI) << std::endl;
                
//                 // Déverrouiller le mutex après utilisation des données
//                 pthread_mutex_unlock(&data_mutex);
                
//                 looptime = micros() - startloop;
//                 usleep(robot.SAMPLING_TIME*1e6 - looptime);
//                 if (looptime > robot.SAMPLING_TIME*1e6) {
//                     std::cout << "Temps de boucle dépassé: " << looptime << std::endl;
//                 }
//                 usleep(robot.SAMPLING_TIME*1e6 - looptime);
//             }
//             break;
//         }
//         case 'c': {
//             std::cout << "Récupération de la vitesse du moteur gauche..." << std::endl;
//             std::cout << "La fonctionnalité de vitesse du robot n'est pas encore implémentée." << std::endl;
//             break;
//         }
//         case 't':{
//             std::cout << "Acquisition des données en boucle ouverte..." << std::endl;
//             robot.openLoopData();
//             break;
//         }
//         case 'd':{
//             std::cout << "Affichage de la distance..." << std::endl;
//             robot.printDistance();
//             break;
//         }
//         case 'l':{
//             std::cout << "Test de bas niveau..." << std::endl;
//             robot.lowLevelTest();
//             break;
//         }
//         case 'f':{
//             std::cout << "Test de freinage..." << std::endl;
//             robot.stop();
//             break;
//         }
//         case 'u':{
//             std::cout << "BZZZ BZZZZZ" << std::endl;
//             robot.buzzBuzzer();
//             break;
//         }
//         case 'm': {
//             std::cout << "Test de niveau moyen..." << std::endl;
            
//             // Obtenir les coordonnées cibles de l'utilisateur
//             float targetX, targetY;
//             std::cout << "Entrez la coordonnée X cible (mètres): ";
//             std::cin >> targetX;
//             std::cout << "Entrez la coordonnée Y cible (mètres): ";
//             std::cin >> targetY;

//             unsigned long startloop;
//             unsigned long looptime;
//             int counter = 0;
//             while (running) {
//                 startloop = micros();
                
//                 // Verrouiller le mutex avant d'accéder aux données
//                 pthread_mutex_lock(&data_mutex);
                
//                 robot.updateOdometry(game);
                
//                 if(counter == 10){
//                     robot.middleLevelController(targetX, targetY, 0, deplacement, game);
//                     counter = 0;
//                 }
//                 counter++;
                
//                 // Déverrouiller le mutex après utilisation des données
//                 pthread_mutex_unlock(&data_mutex);

//                 looptime = micros() - startloop;
//                 if (looptime > robot.SAMPLING_TIME*1e6) {
//                     std::cout << "Temps de boucle dépassé: " << looptime << std::endl;
//                 }
//                 usleep(robot.SAMPLING_TIME*1e6 - looptime);
//             }
//             break;
//         }
//         case 'p': {
//             std::cout << "Envoi d'informations à teensy" << std::endl;
//             robot.teensy_init();
//             std::cout << "Initialisation OK" << std::endl;
//             robot.teensy_send_command(0x06);
//             std::cout << "C'est envoyé" << std::endl;
//             break;
//         }
//         case 'q': {
//             std::cout << "Envoi d'informations à teensy" << std::endl;
//             robot.teensy_init();
//             std::cout << "Initialisation OK" << std::endl;
//             robot.teensy_separate_stack();
//             break;
//         }
//         case 's': {
//             // Afficher en continu les données du LIDAR
//             while (running) {
//                 // Verrouiller le mutex avant d'accéder aux données
//                 pthread_mutex_lock(&data_mutex);
                
//                 if (game->map->cluster_count > 0) {
//                     std::cout << "Position Sauron: (" << game->Sauron->cart_pos->x << ", " 
//                               << game->Sauron->cart_pos->y << ")" << std::endl;
//                     // Emergency_stop(game);
//                 }
                
//                 // Déverrouiller le mutex après utilisation des données
//                 pthread_mutex_unlock(&data_mutex);
                
//                 usleep(100000); // 100ms de délai
//             }
//             break;
//         }
//         default:
//             std::cout << "Option invalide." << std::endl;
//             break;
//     }

//     // Arrêter le thread et nettoyer les ressources
//     // running = false;
//     pthread_join(lidar_thread, NULL);
//     pthread_mutex_destroy(&data_mutex);
//     free_game(game);
    
//     std::cout << "Programme terminé avec succès" << std::endl;
//     return 0;
// }