#include <windows.h>
#include <iostream>


int main()
{
	std::cout << "PID: " << GetCurrentProcessId() << std::endl;
	std::cin.get();
	MessageBoxA(nullptr, "Im just a message box, nothing more", "Hello", MB_OK | MB_ICONINFORMATION);
	return 0;
}
