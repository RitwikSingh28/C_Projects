CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -pedantic -ggdb

CFLAGS += -I./include/

SRC_DIR := ./src
BUILD_DIR := ./build

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
all: build

build: $(BUILD_DIR)/excel_eng

$(BUILD_DIR)/excel_eng: $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(BUILD_DIR)/excel_eng

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BUILD_DIR)/excel_eng
	@echo "Running excel_eng"
	$(BUILD_DIR)/excel_eng $(input) 	# Provide cmdline argument as input_file_path

clean:
	rm -rf $(BUILD_DIR)

