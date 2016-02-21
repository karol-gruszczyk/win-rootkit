#pragma once
#include <windows.h>
#define X86_CODE_LEN 7


class Hook
{
public:
	Hook(DWORD at_address, DWORD to_address);
	~Hook();

	void install_hook();
	void remove_hook();
private:
	bool is_hooked = false;
	DWORD hooking_address;
	DWORD destination_address;
	BYTE x86_code[X86_CODE_LEN];
	BYTE x86_orig_code[X86_CODE_LEN];

	void copy_x86_code(BYTE x86_code[X86_CODE_LEN]);
	void update_jmp_address(DWORD address);
};
