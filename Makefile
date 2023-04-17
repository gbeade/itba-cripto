CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -fsanitize=address
LDFLAGS = -lm

INCLUDE_DIR = include
SRC_DIR = src
BIN_DIR = bin
TEST_DIR = test

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/main

# Rules for compilation

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)
	rm -rf $(BIN_DIR)/*.o

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BIN_DIR)/*

# Rules for testing

TEST_SOURCES := $(wildcard src/*.c) $(wildcard test/*.c) $(wildcard test/include/*.c)
TEST_HEADERS := $(wildcard include/*.h) $(wildcard test/include/*.h)
TEST_OBJECTS := $(addprefix bin/, $(notdir $(TEST_SOURCES:.c=.o)))

bin/%.o: test/%.c $(TEST_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: test/include/%.c $(TEST_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

bin/test: $(TEST_OBJECTS)
	rm -rf $(BIN_DIR)/main.o
	$(CC) $(CFLAGS) bin/*.o -o $@
	rm -rf $(BIN_DIR)/*.o


.PHONY: test

test: bin/test
	./bin/test

valgrind-test: CFLAGS := $(filter-out -fsanitize=address,$(CFLAGS))
valgrind-test: bin/test
	valgrind --leak-check=full --show-leak-kinds=all ./bin/test
