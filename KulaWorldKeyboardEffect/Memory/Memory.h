#pragma once
#include <iostream>

#include <tchar.h>
#include <Windows.h>
#include <TlHelp32.h>

namespace Memory {
	int ReadProcMem(HANDLE processHandle, DWORD addressToRead);
	DWORD_PTR GetModuleBaseAddress(DWORD dwProcID, const char* szModuleName);
	bool AttachProcessHandle(char* procName, HANDLE& processHandle, DWORD& processID, DWORD_PTR& moduleBaseAddress);
}