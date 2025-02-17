# Compiler and flags
CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I. -MMD
LIBS     = -lwiringPi

# Directories
SRC_DIR  = ./src
OBJ_DIR  = ./obj

# Source files (adjust if needed)
SRCS     = $(SRC_DIR)/main.cpp $(SRC_DIR)/Robot.cpp $(SRC_DIR)/Motor.cpp
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
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependency files (generated automatically by -MMD)
-include $(DEPS)

# Clean rule
clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)
