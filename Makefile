cc = g++

lflags = -Wall --std=c++11
cflags = $(lflags) -c

ifeq ($(debug), true)
	cdebug = -g
endif



all: rootkit test



rootkit_target = rootkit.exe
rootkit_cppsources = src/main.cpp src/hook.cpp
rootkit_cppobjects = $(rootkit_cppsources:.cpp=.o)

rootkit: $(rootkit_cppobjects)
	$(cc) $(cdebug) $(lflags) $(rootkit_cppobjects) -o $(rootkit_target)

main.o: main.cpp
	$(cc) $(cdebug) $(cflags) main.cpp

hook.o: hook.cpp hook.h
	$(cc) $(cdebug) $(cflags) hook.cpp



test_target=test.exe

test:
	$(cc) $(cdebug) $(cflags) src/test.cpp -o $(test_target)


clean:
	$(RM) src/*.o
