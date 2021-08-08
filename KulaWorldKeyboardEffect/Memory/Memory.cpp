#include "Memory.h"

namespace Memory {
	int ReadProcMem(HANDLE processHandle, DWORD addressToRead) {
		int rpmBuffer;
		ReadProcessMemory(processHandle, (PVOID)addressToRead, &rpmBuffer, sizeof(int), 0);
		return rpmBuffer;
	}

	DWORD_PTR GetModuleBaseAddress(DWORD dwProcID, const char* szModuleName)
	{
		DWORD_PTR dwModuleBaseAddress = 0;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcID);
		if (hSnapshot != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 ModuleEntry32;
			ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
			if (Module32First(hSnapshot, &ModuleEntry32))
			{
				do
				{
					if (_tcsicmp(ModuleEntry32.szModule, szModuleName) == 0)
					{
						dwModuleBaseAddress = (DWORD_PTR)ModuleEntry32.modBaseAddr;
						break;
					}
				} while (Module32Next(hSnapshot, &ModuleEntry32));
			}
			CloseHandle(hSnapshot);
		}
		return dwModuleBaseAddress;
	}

	bool AttachProcessHandle(char* procName, HANDLE& processHandle, DWORD& processID, DWORD_PTR& moduleBaseAddress) {
		PROCESSENTRY32 procEntry32;
		procEntry32.dwSize = sizeof(PROCESSENTRY32);
		auto processHandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (processHandleSnap == INVALID_HANDLE_VALUE) { std::cout << "ERROR: Failed to Take Snapshot of Process!" << std::endl; return false;}
		while (Process32Next(processHandleSnap, &procEntry32)) {
			if (!strcmp(procName, procEntry32.szExeFile)) {
				processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
				processID = procEntry32.th32ProcessID;
				moduleBaseAddress = GetModuleBaseAddress(processID, "ePSXe.exe");
				if (processHandle == NULL) { std::cout << "ERROR: Failed Getting Handle to Process!" << std::endl; return false; }
				CloseHandle(processHandleSnap);
				return true;
			}
		}
		std::cout << "ERROR: Failed to Find " << procName << " in the Process Snapshot!" << std::endl;
		CloseHandle(processHandleSnap);
		return false;
	}
}