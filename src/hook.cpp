#include "hook.h"

Hook* Hook::instance;

Hook* Hook::get_instance()
{
	if (!instance)
		instance = new Hook();
	return instance;
}

void Hook::install_hook(DWORD at_address, DWORD to_address)
{
	update_jmp_address(to_address);

	DWORD old_protect;
	VirtualProtect((LPVOID)at_address, 1024, PAGE_EXECUTE_READWRITE, &old_protect);
	memcpy((void*)at_address, x86_code, X86_CODE_LEN);
	VirtualProtect((LPVOID)at_address, 1024, old_protect, nullptr);
}

Hook::Hook()
{
	// mov <address>
	x86_code[0] = 0xb8;

	// jmp eax
	x86_code[5] = 0xff;
	x86_code[6] = 0xe0;
}

Hook::~Hook()
{
	if (instance)
		delete instance;
}

void Hook::update_jmp_address(DWORD address)
{
	x86_code[1] = address & 0xFF;
	x86_code[2] = (address >> 8) & 0xFF;
	x86_code[3] = (address >> 16) & 0xFF;
	x86_code[4] = (address >> 24) & 0xFF;
}
