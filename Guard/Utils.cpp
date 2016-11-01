#include "Utils.hpp"

// Spawn a CLI window and redirect I/O
void Utils::Console::Spawn(const char* title)
{
	AllocConsole();
	FILE *conin, *conout;

	freopen_s(&conin, "conin$", "r", stdin);
	freopen_s(&conout, "conout$", "w", stderr);
	freopen_s(&conout, "conout$", "w", stdout);

	if (title != nullptr)
		SetConsoleTitleA(title);
}

// Output text to console
void Utils::Console::Output(string text)
{
	std::cout << text << "\n";
}

// Output text to console with optional colours
void Utils::Console::Output(string text, WindowColour PreColor, WindowColour PostColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PreColor);
	Utils::Console::Output(text);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PostColor);
}

// Get random INT value between min and max
int Utils::Random(int min, int max)
{
	int range = max - min;
	int random = min + int(range*rand()) / (RAND_MAX + 1);
	return random;
}


typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL Utils::Is64Bit(HANDLE hProcess)
{
	BOOL bIsWow64 = FALSE;

	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(hProcess, &bIsWow64))
		{
			// handle error, must 32bit right? ;D
			return false;
		}
	}
	return bIsWow64;
}

char* Utils::ProcessArchitecture(HANDLE hProcess) {
	return Utils::Is64Bit(hProcess) ? "64bit" : "32bit";
}
