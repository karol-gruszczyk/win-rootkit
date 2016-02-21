#include <windows.h>
#include <memory>
#include <iostream>
#include <stdexcept>

#include "hook.h"



BOOL WINAPI DllMain(HINSTANCE dllInst, DWORD reason, LPVOID reserved)
{
	TCHAR process_name[MAX_PATH];
	GetModuleFileName(nullptr, process_name, MAX_PATH);
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		MessageBox(nullptr, process_name, "Hook loaded", MB_OK | MB_ICONINFORMATION);
		break;
	case DLL_PROCESS_DETACH:
		MessageBox(nullptr, process_name, "Hook unloaded", MB_OK | MB_ICONINFORMATION);
		break;
	}
	return TRUE;
}
