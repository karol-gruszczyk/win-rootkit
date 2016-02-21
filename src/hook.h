#pragma once
#include <windows.h>
#define X86_CODE_LEN 7


class Hook
{
public:
	static Hook* get_instance();

	void install_hook(DWORD at_address, DWORD to_address);
private:
	Hook();
	~Hook();

	static Hook* instance;
	BYTE x86_code[X86_CODE_LEN];

	void update_jmp_address(DWORD address);
};
