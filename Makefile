ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/usr/local/Cellar/glfw3/3.2.1/lib -framework OpenGL -lglfw3 -lm
else
LDFLAGS=-lX11 -lGL -lGLU -lglfw -lGLEW -lm
endif
CC = g++
CFLAGS=-g -Wall -I/usr/local/Cellar/glfw3/3.2.1/include

TARGET = flocking
SRC = $(TARGET).cc
LIB = list.o boid.o view.o
all: flock

flock: $(SRC) $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC) $(LIB)

list.o: list.cc list.h
	$(CC) $(CFLAGS) -c list.cc

boid.o: boid.cc boid.h
	$(CC) $(CFLAGS) -c boid.cc

view.o: view.cc view.h
	$(CC) $(CFLAGS) -c view.cc

clean:
	rm $(TARGET) $(LIB)
