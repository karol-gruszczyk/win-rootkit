cc = g++

flags = -Wall --std=c++11

ifeq ($(debug), true)
	flags += -g
endif



all: rootkit test



rootkit_target = rootkit.exe
rootkit_cppsources = src/main.cpp src/dll_injector.cpp
rootkit_cppobjects = $(rootkit_cppsources:.cpp=.o)

rootkit: $(rootkit_cppobjects)
	$(cc) $(flags) $(rootkit_cppobjects) -o $(rootkit_target)

src/main.o: src/main.cpp
	$(cc) $(flags) -c src/main.cpp -o src/main.o

src/dll_injector.o: src/dll_injector.cpp src/dll_injector.h
	$(cc) $(flags) -c src/dll_injector.cpp -o src/dll_injector.o



test_target=test.exe

test:
	$(cc) $(flags) src/test.cpp -o $(test_target)


clean:
	$(RM) src/*.o
