#include <windows.h>

int main()
{
	MessageBoxA(nullptr, "Im just a message box, nothing more", "Hello", MB_OK | MB_ICONINFORMATION);
	return 0;
}
