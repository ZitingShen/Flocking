ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/usr/local/Cellar/glfw3/3.2.1/lib -framework OpenGL -lglfw3 -lm
else
LDFLAGS=-lX11 -lGL -lGLU -lglfw -lGLEW -lm
endif
CC = gcc
CFLAGS=-g -Wall -I/usr/local/Cellar/glfw3/3.2.1/include

TARGET = flock
SRC = $(flock).c
LIB = list.o boid.o
all: flock

flock: $(SRC) $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC) $(LIB)

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

boid.o: boid.c boid.h
	$(CC) $(CFLAGS)- c boid.c