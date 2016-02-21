cc = g++

flags = -Wall --std=c++11 -static-libgcc -static-libstdc++
dllflags = -Wall --std=c++11

ifeq ($(debug), true)
	flags += -g
endif



all: rootkit test dll



rootkit_target = rootkit.exe
rootkit_cppsources = src/main.cpp src/dll_injector.cpp
rootkit_cppobjects = $(rootkit_cppsources:.cpp=.o)

rootkit: $(rootkit_cppobjects)
	$(cc) $(flags) $(rootkit_cppobjects) -o $(rootkit_target) -Wl,-subsystem,windows

src/main.o: src/main.cpp
	$(cc) $(flags) -c src/main.cpp -o src/main.o

src/dll_injector.o: src/dll_injector.cpp src/dll_injector.h
	$(cc) $(flags) -c src/dll_injector.cpp -o src/dll_injector.o



dll_target = win-rootkit.dll
dll_cppsources = src/dllmain.cpp src/hook.cpp
dll_cppobjects = $(dll_cppsources:.cpp=.o)

dll: $(dll_cppobjects)
	$(cc) $(dllflags) $(dll_cppobjects) -shared -o $(dll_target)

src/dllmain.o: src/dllmain.cpp
	$(cc) $(dllflags) -c src/dllmain.cpp -o src/dllmain.o -DBUILDING_EXAMPLE_DLL

src/hook.o: src/hook.cpp
	$(cc) $(dllflags) -c src/hook.cpp -o src/hook.o -DBUILDING_EXAMPLE_DLL



test_target=test.exe

test:
	$(cc) $(flags) src/test.cpp -o $(test_target)


clean:
	$(RM) src/*.o
