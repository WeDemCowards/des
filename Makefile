# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# The target executable
TARGET = des

# The source files (located in the src/ directory)
SRCS = src/main.c src/des.c src/keygen.c

# The object files (derived from the source files, placed in the build/ directory)
OBJS = $(SRCS:src/%.c=build/%.o)

# Default target
all: $(TARGET)

# Rule to link the object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Rule to compile .c files into .o files
build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the object files and executable
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean

