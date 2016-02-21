#include "hook.h"

void Hook::install_hook()
{
	if (is_hooked)
		return;
	// code backup for removing hook
	memcpy((void*)x86_orig_code, (void*)hooking_address, X86_CODE_LEN);

	copy_x86_code(x86_code);

	is_hooked = true;
}

void Hook::remove_hook()
{
	if (!is_hooked)
		return;
	copy_x86_code(x86_orig_code);
	is_hooked = false;
}

Hook::Hook(DWORD at_address, DWORD to_address)
	: hooking_address(at_address), destination_address(to_address)
{
	// mov eax, <address>(4 bytes)
	x86_code[0] = 0xB8;
	
	update_jmp_address(destination_address);

	// jmp eax
	x86_code[5] = 0xFF;
	x86_code[6] = 0xE0;
}

Hook::~Hook()
{
	if (is_hooked)
		remove_hook();
}

void Hook::copy_x86_code(BYTE x86_code[X86_CODE_LEN])
{
	DWORD old_protect;
	VirtualProtect((LPVOID)hooking_address, 1024, PAGE_EXECUTE_READWRITE, &old_protect);
	memcpy((void*)hooking_address, x86_code, X86_CODE_LEN);
	VirtualProtect((LPVOID)hooking_address, 1024, old_protect, nullptr);
}

void Hook::update_jmp_address(DWORD address)
{
	x86_code[1] = (BYTE)address;
	x86_code[2] = (BYTE)(address >> 8);
	x86_code[3] = (BYTE)(address >> 16);
	x86_code[4] = (BYTE)(address >> 24);
}
