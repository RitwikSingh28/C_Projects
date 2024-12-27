CC := gcc
CFLAGS := -g -W -Wall -Wextra

SRC_DIR := ./src
BUILD_DIR := ./build

# Default target
all: build

# Build target - link the object file to create the executable
build: $(BUILD_DIR)/main.o
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(BUILD_DIR)/main.o -o $(BUILD_DIR)/main

# Rule to compile main.c into main.o
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@

# Clean target - remove build directory
clean:
	rm -rf $(BUILD_DIR)

