#include <windows.h>
#include <memory>
#include <iostream>

#include "hook.h"


typedef int (WINAPI *MessageBoxAPtr)(HWND, LPCSTR, LPCSTR, UINT);

int WINAPI OverloadedMessageBoxA(HWND hwnd, LPCSTR text, LPCSTR title, UINT type)
{
	std::cout << "test test" << std::endl;
	return 0;
}

void test()
{
	MessageBoxA(nullptr, "test", "test", 0);
}

int main()
{
	test();
	DWORD oldFuncPtr = reinterpret_cast<DWORD>(MessageBoxA);
	DWORD newFuncPtr = reinterpret_cast<DWORD>(OverloadedMessageBoxA);
	Hook::get_instance()->install_hook(oldFuncPtr, newFuncPtr);
	test();
	system("pause");
	return 0;
}
