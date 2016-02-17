# the compiler: gcc for C, g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings

CFLAGS=-g -Wall


all: main.o
	$(CC) main.o -o rootkit.out

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	$(RM) -f *.o
