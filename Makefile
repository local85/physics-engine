# Compiler
CXX = g++

# Homebrew paths
GLFW_PATH = /opt/homebrew/opt/glfw
GLM_PATH = /opt/homebrew/opt/glm

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -I$(GLFW_PATH)/include -I$(GLM_PATH)/include

# Linker flags
LDFLAGS = -L$(GLFW_PATH)/lib

# Libraries
LIBS = -lglfw -framework OpenGL

# Target executable
TARGET = engine

# Source files
SOURCES = engine.cpp Object.cpp Circle.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
