# Makefile for NEA

CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic
LDFLAGS := 
SRC_DIR := src
INCLUDE_DIR := include
GLAD_DIR := external/glad
BUILD_DIR := build
TARGET := NEA

# Grab all .cpp files recursively
SOURCES := $(shell find $(SRC_DIR) -name "*.cpp")
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# GLAD
GLAD_SRC := $(GLAD_DIR)/src/glad.c
GLAD_OBJ := $(BUILD_DIR)/glad.o
GLAD_INC := -I$(GLAD_DIR)/include

# GLFW and OpenGL via pkg-config
PKG_CFLAGS := $(shell pkg-config --cflags glfw3)
PKG_LIBS := $(shell pkg-config --libs glfw3) -lGL

# Include dirs
INCLUDES := -I$(INCLUDE_DIR) $(GLAD_INC) $(PKG_CFLAGS)

# Default target
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJECTS) $(GLAD_OBJ)
	@mkdir -p $(@D)
	$(CXX) $(OBJECTS) $(GLAD_OBJ) -o $@ $(PKG_LIBS) $(LDFLAGS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile GLAD
$(GLAD_OBJ): $(GLAD_SRC)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(GLAD_DIR)/include -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Run
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
