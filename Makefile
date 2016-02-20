CFILES = main.o renderCylinder.o
CFLAGS = -O3 -Wall `pkg-config --cflags glfw3`
LIBS = -lGLU -lm `pkg-config --static --libs glfw3`
CC = g++
NAME = prog

all: main

main:	$(CFILES)
	$(CC) $(CFILES) $(CFLAGS) -o $(NAME) $(LIBS)

main.o: main.cpp
	$(CC) -c main.cpp

renderCylinder.o: renderCylinder.cpp
	$(CC) -c renderCylinder.cpp

clean:
	rm -f *~ *.o *.obj $(NAME)
