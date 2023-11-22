UNAME_S:=$(shell uname -s)
CC=gcc
CFLAGS=-std=c2x 
INCLUDE_DIR=include
GLFW_LIB_DIR=$(INCLUDE_DIR)/GLFW/build/src/
GLEW_LIB_DIR=$(INCLUDE_DIR)/GL/lib/
BUILD_DIR=build
LDFLAGS= -g -O0 -Wall -Wextra -pedantic -lpthread -ldl -lm $(GLEW_LIB_DIR)libGLEW.a $(GLFW_LIB_DIR)libglfw3.a 

#List of directories containing source files
SRC_DIRS=src/engine src/atari_combat

# Create a list of source files in each directory
SRC+=$(shell find $(SRC_DIRS) -type f -name '*.c')

EXECUTABLE=$(BUILD_DIR)/app

ifeq ($(UNAME_S),Darwin)
    LDFLAGS += -framework OpenGL -framework CoreVideo -framework Cocoa -framework IOKit
else 
	LDFLAGS += -lGL
endif

$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) src/main.c $^ -o $@ -I$(INCLUDE_DIR) $(LDFLAGS)  

.PHONY: clean
clean:
	rm -f $(EXECUTABLE)

