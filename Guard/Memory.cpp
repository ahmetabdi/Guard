#include "SDK.hpp"
#include "Memory.hpp"

//-----------------------------------------------------------------------------------------------------------------------------
// Purpose: HANDLE to a specified process
//-----------------------------------------------------------------------------------------------------------------------------
HANDLE Memory::hProcess = NULL;

//-----------------------------------------------------------------------------------------------------------------------------
// Purpose: DWORD to hold the ProcessID of a specified process
//-----------------------------------------------------------------------------------------------------------------------------
DWORD Memory::dwPID = NULL;

//-----------------------------------------------------------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------------------------------------------------------
Memory::Memory()
{

}

//-----------------------------------------------------------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------------------------------------------------------
Memory::~Memory()
{
	CloseHandle(hProcess);
}

//-----------------------------------------------------------------------------------------------------------------------------
// Purpose: Gets a handle to a given process, returns true if it succeeds, returns false if it fails
// Usage: Memory::AttachProcess("Process.exe");
//-----------------------------------------------------------------------------------------------------------------------------
bool Memory::AttachProcess(char* Name)
{
	HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 ProcessEntry;
	ProcessEntry.dwSize = sizeof(ProcessEntry);

	do
		if (!strcmp(ProcessEntry.szExeFile, Name))
		{
			Memory::dwPID = ProcessEntry.th32ProcessID;
			CloseHandle(hPID);

			Memory::hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Memory::dwPID);

			return true;
		}
		else
			Sleep(1);
	while (Process32Next(hPID, &ProcessEntry));

	return false;
}

//-----------------------------------------------------------------------------------------------------------------------------
// Purpose: Returns information about a given Module as MODULEENTRY32, returns 0x0 as modBaseAddr if it fails
// Usage: MODULEENTRY32 a = Memory::GetModuleInformations("Module.dll");
//-----------------------------------------------------------------------------------------------------------------------------
MODULEENTRY32 Memory::GetModuleInformations(LPSTR Module)
{
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Memory::dwPID);
	MODULEENTRY32 ModuleEntry;
	ModuleEntry.dwSize = sizeof(ModuleEntry);

	do
		if (!strcmp(ModuleEntry.szModule, Module))
		{
			CloseHandle(hModule);
			return ModuleEntry;
		}
	while (Module32Next(hModule, &ModuleEntry));

	ModuleEntry.modBaseAddr = 0x0;

	return ModuleEntry;
}

//-----------------------------------------------------------------------------------------------------------------------------
// Purpose: Compares the Pattern with the Mask
//-----------------------------------------------------------------------------------------------------------------------------
bool Memory::CompareData(const unsigned char* pbData, const unsigned char* pbMask, const char* pszString)
{
	for (; *pszString; ++pszString, ++pbData, ++pbMask)
		if (*pszString == 'x' && *pbData != *pbMask)
			return FALSE;
	return (*pszString) == NULL;
}

DWORD Memory::FindPattern(DWORD start, DWORD size, const char* sig, const char* mask)
{
	BYTE* data = new BYTE[size];

	unsigned long bytesRead;

	if (!ReadProcessMemory(hProcess, (LPVOID)start, data, size, &bytesRead)) {
		return NULL;
	}

	for (DWORD i = 0; i < size; i++) {
		if (CompareData((const BYTE*)(data + i), (const BYTE*)sig, mask)) {
			return start + i;
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------------------------------------------------------
// Purpose: Returns an Offset by Pattern & Mask as DWORD
// Usage: DWORD a = Memory::GetOffset("Module.dll", { MyPattern }, "MyMask");
//-----------------------------------------------------------------------------------------------------------------------------
DWORD Memory::GetOffset(char* Module, char* Mask, char* Pattern, BYTE Offset, bool SubtractClientDLL)
{
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Memory::dwPID);
	MODULEENTRY32 ModuleInformation;

	do
		if (!strcmp(ModuleInformation.szModule, Module))
		{
			CloseHandle(hModule);
		}
	while (Module32Next(hModule, &ModuleInformation));

	DWORD Address = FindPattern((DWORD)ModuleInformation.modBaseAddr, (DWORD)ModuleInformation.modBaseSize, Pattern, Mask);

	DWORD dwOffset = Memory::ReadMemory<DWORD>(Address + Offset);

	if (!SubtractClientDLL)
		return dwOffset;
	else
		return dwOffset - (DWORD)ModuleInformation.modBaseAddr;
}
