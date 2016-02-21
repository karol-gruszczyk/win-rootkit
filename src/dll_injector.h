#pragma once
#include <windows.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>


class DllInjector
{
public:
	DllInjector(std::string dll_path);

	void inject_into_pid(int pid);
private:
	std::string dll_path;

	bool file_exists(std::string& path);
	void enable_debug_privileges();
	HANDLE open_process(int pid);
	void* allocate_process_memory(int pid, HANDLE process_handle);
	void write_process_memory(int pid, HANDLE process_handle, void* dll_path_memory);
	HANDLE create_remote_thread(int pid, HANDLE process_handle, void* dll_path_memory);
	void run_thread(HANDLE thread_handle);
};

