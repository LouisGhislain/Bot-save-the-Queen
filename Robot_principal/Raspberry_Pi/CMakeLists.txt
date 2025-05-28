cmake_minimum_required(VERSION 3.25.1)
project(exeRobotProgram)

# Définir le standard C++
set(CMAKE_CXX_STANDARD 17)

# Inclure les répertoires d'en-tête du LIDAR
include_directories(lib/rplidar_sdk/sdk/include)
include_directories(src)

# Rechercher récursivement tous les fichiers .h et .hpp dans le répertoire include
file(GLOB_RECURSE HEADER_FILES "include/*.h" "include/*.hpp")

# Extraire les répertoires uniques des fichiers d'en-tête
foreach(HEADER ${HEADER_FILES})
    get_filename_component(HEADER_DIR ${HEADER} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${HEADER_DIR})
endforeach()

# Supprimer les répertoires en double
list(REMOVE_DUPLICATES INCLUDE_DIRS)

# Ajouter les répertoires d'inclusion
include_directories(${INCLUDE_DIRS})

# Ajouter les fichiers source spécifiques du LIDAR
set(LIDAR_SOURCES
    src/Struct.cpp
    src/lidar.cpp
)

# Ajouter les fichiers d'en-tête spécifiques du LIDAR
set(LIDAR_HEADERS
    include/struct.h
    include/lidar.h
)

# Rechercher récursivement tous les fichiers .cpp dans src
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_compile_options(-Wno-psabi)

# Ajouter l'exécutable
add_executable(exeRobotProgram ${SOURCES} ${LIDAR_HEADERS})

# Trouver la bibliothèque WiringPi
find_library(WIRINGPI_LIBRARY wiringPi)

# Vérifier si WiringPi a été trouvé
if (NOT WIRINGPI_LIBRARY)
    message(FATAL_ERROR "Bibliothèque WiringPi non trouvée!")
endif()

# Lier les bibliothèques WiringPi et RPLIDAR à votre exécutable
target_link_libraries(exeRobotProgram
    ${WIRINGPI_LIBRARY}
    ${CMAKE_SOURCE_DIR}/lib/rplidar_sdk/output/Linux/Release/libsl_lidar_sdk.a
    pthread
)

# Afficher les messages d'état
message(STATUS "Projet Robot avec LIDAR configuré avec succès.")