#include <windows.h>
#include <memory>
#include <iostream>
#include <stdexcept>

#include "hook.h"


typedef int (WINAPI *MessageBoxAPtr)(HWND, LPCSTR, LPCSTR, UINT);
Hook* hook;
HMODULE hmodule;
DWORD oldFuncPtr;
DWORD newFuncPtr;

int WINAPI OverloadedMessageBoxA(HWND hwnd, LPCSTR text, LPCSTR title, UINT type)
{
	hook->remove_hook();
	int ret = MessageBoxA(hwnd, "HACKED LOL", "H4CK", type);
	hook->install_hook();
	return ret;
}

BOOL WINAPI DllMain(HINSTANCE dllInst, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		hmodule = GetModuleHandleA("user32.dll");
		oldFuncPtr = reinterpret_cast<DWORD>(GetProcAddress(hmodule, "MessageBoxA"));
		newFuncPtr = reinterpret_cast<DWORD>(&OverloadedMessageBoxA);
		hook = new Hook(oldFuncPtr, newFuncPtr);
		hook->install_hook();
		break;
	case DLL_PROCESS_DETACH:
		hook->remove_hook();
		delete hook;
		break;
	}
	return TRUE;
}
