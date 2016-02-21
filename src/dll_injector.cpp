#include "dll_injector.h"


DllInjector::DllInjector(std::string dll_path)
	: dll_path(dll_path)
{
	enable_debug_privileges();
}


void DllInjector::inject_into_pid(int pid)
{
	if (!file_exists(dll_path))
		throw std::runtime_error("File '" + dll_path + "' does not exist!");

	HANDLE process = open_process(pid);
	void* memory_ptr = allocate_process_memory(pid, process);
	write_process_memory(pid, process, memory_ptr);
	HANDLE thread = create_remote_thread(pid, process, memory_ptr);
	run_thread(thread);
}

bool DllInjector::file_exists(std::string& path)
{
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}

void DllInjector::enable_debug_privileges()
{
	HANDLE              hToken;
	LUID                SeDebugNameValue;
	TOKEN_PRIVILEGES    TokenPrivileges;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &SeDebugNameValue))
		{
			TokenPrivileges.PrivilegeCount = 1;
			TokenPrivileges.Privileges[0].Luid = SeDebugNameValue;
			TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			if (AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
				CloseHandle(hToken);
			else
			{
				CloseHandle(hToken);
				throw std::runtime_error("Couldn't adjust token privileges!");
			}
		}
		else
		{
			CloseHandle(hToken);
			throw std::runtime_error("Couldn't look up privilege value!");
		}
	}
	else
		throw std::runtime_error("Couldn't open process token!");
}

HANDLE DllInjector::open_process(int pid)
{
	HANDLE remote_process = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (!remote_process)
	{
		int error = GetLastError();
		std::ostringstream stream;
		stream << "Failed to open process(pid: " << pid << "). Error code: " << error;
		throw std::runtime_error(stream.str());
	}
	return remote_process;
}

void* DllInjector::allocate_process_memory(int pid, HANDLE process_handle)
{
	void* dll_path_memory = VirtualAllocEx(process_handle, nullptr, dll_path.size() + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!dll_path_memory)
	{
		int error = GetLastError();
		std::ostringstream stream;
		stream << "Failed to allocate memory process(pid: " << pid << "). Error code: " << error;
		throw std::runtime_error(stream.str());
	}
	return dll_path_memory;
}

void DllInjector::write_process_memory(int pid, HANDLE process_handle, void* dll_path_memory)
{
	if (!WriteProcessMemory(process_handle, dll_path_memory, dll_path.c_str(), dll_path.size() + 1, nullptr))
	{
		int error = GetLastError();
		std::ostringstream stream;
		stream << "Failed to allocate memory process(pid: " << pid << "). Error code: " << error;
		throw std::runtime_error(stream.str());
	}
}

HANDLE DllInjector::create_remote_thread(int pid, HANDLE process_handle, void* dll_path_memory)
{
	LPTHREAD_START_ROUTINE thread_start_routine = (LPTHREAD_START_ROUTINE)(GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"));
	HANDLE remote_thread = CreateRemoteThread(process_handle, nullptr, 0, thread_start_routine, dll_path_memory, CREATE_SUSPENDED, nullptr);
	if (!remote_thread)
	{
		int error = GetLastError();
		std::ostringstream stream;
		stream << "Failed to create remote thread(pid: " << pid << "). Error code: " << error;
		throw std::runtime_error(stream.str());
	}
	return remote_thread;
}

void DllInjector::run_thread(HANDLE thread_handle)
{
	ResumeThread(thread_handle);
	WaitForSingleObject(thread_handle, INFINITE);
}
