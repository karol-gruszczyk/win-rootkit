# the compiler: gcc for C, g++ for C++
CC=cl.exe
LINK=link.exe

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings

CFLAGS=-g -Wall --std=c++11

TARGET=rootkit.exe
TEST_TARGET=test.exe


all: main.o
	$(CC) main.o -o $(TARGET)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp


test:
	$(CC) $(CFLAGS) test.cpp -o $(TEST_TARGET)


clean:
	$(RM) *.o
