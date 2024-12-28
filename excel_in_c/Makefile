CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -pedantic -ggdb

SRC_DIR := ./src
BUILD_DIR := ./build

all: build

build: $(BUILD_DIR)/excel_eng

$(BUILD_DIR)/excel_eng: $(BUILD_DIR)/main.o
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(BUILD_DIR)/main.o -o $(BUILD_DIR)/excel_eng

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@

run: $(BUILD_DIR)/excel_eng
	@echo "Running excel_eng"
	$(BUILD_DIR)/excel_eng $(input) 	# Provide cmdline argument as input_file_path

clean:
	rm -rf $(BUILD_DIR)

