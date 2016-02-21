cc = g++

flags = -Wall --std=c++11

ifeq ($(debug), true)
	flags += -g
endif



all: rootkit test



rootkit_target = rootkit.exe
rootkit_cppsources = src/main.cpp src/hook.cpp
rootkit_cppobjects = $(rootkit_cppsources:.cpp=.o)

rootkit: $(rootkit_cppobjects)
	$(cc) $(flags) $(rootkit_cppobjects) -o $(rootkit_target)

src/main.o: src/main.cpp
	$(cc) $(flags) -c src/main.cpp -o src/main.o

src/hook.o: src/hook.cpp src/hook.h
	$(cc) $(flags) -c src/hook.cpp -o src/hook.o



test_target=test.exe

test:
	$(cc) $(flags) src/test.cpp -o $(test_target)


clean:
	$(RM) src/*.o
