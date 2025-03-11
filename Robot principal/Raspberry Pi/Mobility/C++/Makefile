# Compiler and flags
CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I. -Iinclude -MMD
LIBS     = -lwiringPi -lwiringPiDev

# Directories
SRC_DIR  = ./src
OBJ_DIR  = ./obj

# Source files (ajout de Screen.cc)
SRCS     = $(SRC_DIR)/main.cpp $(SRC_DIR)/Robot.cpp $(SRC_DIR)/Motor.cpp $(SRC_DIR)/Screen.cpp
OBJS     = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS     = $(OBJS:.o=.d)

# Final executable name
TARGET   = robot_program

# Default target
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Compilation rule for .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Include dependency files (generated automatically by -MMD)
-include $(DEPS)

# Clean rule
clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)
