#pragma once

#include "SDK.hpp"

//-----------------------------------------------------------------------------------------------------------------------------
// Purpose: Contains various functions for memory manipulation
// Usage: Memory::
//-----------------------------------------------------------------------------------------------------------------------------
class Memory {
private:
	//-----------------------------------------------------------------------------------------------------------------------------
	// Purpose: Compares the Pattern with the Mask
	//-----------------------------------------------------------------------------------------------------------------------------
	static bool CompareData(const unsigned char* pbData, const unsigned char* pbMask, const char* pszString);

	//-----------------------------------------------------------------------------------------------------------------------------
	// Purpose: Finds the Pattern in Memory and reads the address for the Pattern
	//-----------------------------------------------------------------------------------------------------------------------------
	static DWORD FindPattern(DWORD start, DWORD size, const char* sig, const char* mask);
protected:
	
public:
	//-----------------------------------------------------------------------------------------------------------------------------
	// Purpose: HANDLE to a specified process
	//-----------------------------------------------------------------------------------------------------------------------------
	static HANDLE hProcess;

	//-----------------------------------------------------------------------------------------------------------------------------
	// Purpose: DWORD to hold the ProcessID of a specified process
	//-----------------------------------------------------------------------------------------------------------------------------
	static DWORD dwPID;

	//-----------------------------------------------------------------------------------------------------------------------------
	// Purpose: Constructor
	//-----------------------------------------------------------------------------------------------------------------------------
	Memory();

	//-----------------------------------------------------------------------------------------------------------------------------
	// Purpose: Destructor
	//-----------------------------------------------------------------------------------------------------------------------------
	~Memory();

	//-----------------------------------------------------------------------------------------------------------------------------
	// Purpose: Reads a given address in memory and returns its value as a specified datatype
	// Usage: MyDataType a = Memory::ReadMemory<MyDataType>(AddressToRead);
	//-----------------------------------------------------------------------------------------------------------------------------
	template <class vData>
	static vData ReadMemory(DWORD Address)
	{
		vData vReturn;
		ReadProcessMemory(hProcess, (LPVOID)Address, &vReturn, sizeof(vData), NULL);
		return vReturn;
	}

	//-----------------------------------------------------------------------------------------------------------------------------
	// Purpose: Writes given data to a given address in memory as a given datatype, returns true if it succeeds, returns false if it fails
	// Usage: Memory::WriteMemory<MyDataType>(AddressToWriteTo, DataToWrite);
	//-----------------------------------------------------------------------------------------------------------------------------
	template <class vData>
	static bool WriteMemory(DWORD Address, vData Value)
	{
		if (WriteProcessMemory(hProcess, (LPVOID)Address, &Value, sizeof(vData), NULL)) { return true; }
		else return false;
	}

	//-----------------------------------------------------------------------------------------------------------------------------
	// Purpose: Gets a handle to a given process, returns true if it succeeds, returns false if it fails
	// Usage: Memory::AttachProcess("Process.exe");
	//-----------------------------------------------------------------------------------------------------------------------------
	static bool AttachProcess(char* Name);

	//-----------------------------------------------------------------------------------------------------------------------------
	// Purpose: Returns information about a given Module as MODULEENTRY32, returns 0x0 as modBaseAddr if it fails
	// Usage: MODULEENTRY32 a = Memory::GetModuleInformation("Module.dll");
	//-----------------------------------------------------------------------------------------------------------------------------
	static MODULEENTRY32 GetModuleInformations(LPSTR Module);

	//-----------------------------------------------------------------------------------------------------------------------------
	// Purpose: Returns an Offset by Pattern & Mask
	// Usage: DWORD a = Memory::GetOffset("Module.dll", "MyMask", "MyPattern", 0, true);
	//-----------------------------------------------------------------------------------------------------------------------------
	static DWORD GetOffset(char* Module, char* Mask, char* Pattern, BYTE Offset, bool SubtractClientDLL);
};
