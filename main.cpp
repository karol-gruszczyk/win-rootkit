#include <windows.h>
#include <memory>
#include <iostream>


BYTE x86_code[7];


int WINAPI OurMessageBoxA(HWND hwnd,LPCSTR text,LPCSTR title,UINT type) 
{
	std::cout << "chuj dupa" << std::endl;
	return 0;
}


void initialize_x86_code(DWORD jmp_to_address)
{
	//mov eax, jmp_to_address
	x86_code[0] = 0xb8;
	x86_code[1] = jmp_to_address & 0xFF;
	x86_code[2] = (jmp_to_address >> 8) & 0xFF;
	x86_code[3] = (jmp_to_address >> 16) & 0xFF;
	x86_code[4] = (jmp_to_address >> 24) & 0xFF;
 
	//jmp eax
	x86_code[5] = 0xff;
	x86_code[6] = 0xe0;
}

void injectJMP()
{
	typedef int (WINAPI *MessageBoxAPtr)(HWND, LPCSTR, LPCSTR, UINT);
	MessageBoxAPtr origFuncPtr = reinterpret_cast<MessageBoxAPtr>(GetProcAddress(GetModuleHandleA("user32.dll"), "MessageBoxA"));
    MessageBoxAPtr newFuncPtr= &OurMessageBoxA;
    initialize_x86_code(reinterpret_cast<DWORD>(newFuncPtr));

	DWORD oldProtection;
	VirtualProtect((LPVOID)origFuncPtr, 1024, PAGE_EXECUTE_READWRITE, &oldProtection);
	memcpy((void*)origFuncPtr, x86_code, 7);
	VirtualProtect((LPVOID)origFuncPtr, 1024, oldProtection, nullptr);
}




int main()
{
	system("pause");
	injectJMP();
	system("pause");
	return 0;
}
