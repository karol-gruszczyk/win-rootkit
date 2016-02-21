#include <windows.h>
#include <memory>
#include <iostream>

#include "hook.h"


typedef int (WINAPI *MessageBoxAPtr)(HWND, LPCSTR, LPCSTR, UINT);
Hook* hook;

int WINAPI OverloadedMessageBoxA(HWND hwnd, LPCSTR text, LPCSTR title, UINT type)
{
	hook->remove_hook();
	int ret = MessageBoxA(hwnd, "HACKED LOL", "H4CK", type);
	hook->install_hook();
	return ret;
}

void test()
{
	MessageBoxA(nullptr, "test", "test", 0);
}

int main()
{
	HMODULE hmodule = GetModuleHandleA("user32.dll");
	DWORD oldFuncPtr = reinterpret_cast<DWORD>(GetProcAddress(hmodule, "MessageBoxA"));
	DWORD newFuncPtr = reinterpret_cast<DWORD>(&OverloadedMessageBoxA);
	test();
	hook = new Hook(oldFuncPtr, newFuncPtr);
	hook->install_hook();
	test();
	delete hook;
	return 0;
}
