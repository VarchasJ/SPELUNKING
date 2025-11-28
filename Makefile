# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./src
LDFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11

# Directories
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target executable
TARGET = $(BUILD_DIR)/spelunking_area1

# Default target
all: $(TARGET)

# Create build directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Run the game
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
