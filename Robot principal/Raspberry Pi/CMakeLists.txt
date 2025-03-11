cmake_minimum_required(VERSION 3.25.1)

project(exeRobotProgram) # name of the project 

set(CMAKE_CXX_STANDARD 17)

# Recursively find all .h and .hpp files in the include directory
file(GLOB_RECURSE HEADER_FILES "include/*.h" "include/*.hpp")

# Extract unique directories from the header files
foreach(HEADER ${HEADER_FILES})
    get_filename_component(HEADER_DIR ${HEADER} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${HEADER_DIR})
endforeach()

# Remove duplicate directories
list(REMOVE_DUPLICATES INCLUDE_DIRS)

# Add the include directories
include_directories(${INCLUDE_DIRS})

# Recursively find all .cpp files in src
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add executable
add_executable(exeRobotProgram ${SOURCES})

# Find the WiringPi library
find_library(WIRINGPI_LIBRARY wiringPi)

# Check if WiringPi was found
if (NOT WIRINGPI_LIBRARY)
    message(FATAL_ERROR "WiringPi library not found!")
endif()

# Link the WiringPi library to your executable
target_link_libraries(exeRobotProgram ${WIRINGPI_LIBRARY})
