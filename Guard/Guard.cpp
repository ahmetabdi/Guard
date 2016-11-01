// Guard.cpp : Defines the entry point for the console application.
//

#include "SDK.hpp"
#include "Utils.hpp"
#include "Memory.hpp"

int main()
{
	Utils::Console::Spawn("Log");
	
	char* processName = "swtor.exe";

	if (Memory::AttachProcess(processName)) {

		Utils::Console::Output("------------------------------", WindowColour::GREEN);

		Utils::Console::Output("Process:", WindowColour::GRAY);
		Utils::Console::Output(processName, WindowColour::WHITE);

		Utils::Console::Output("Architecture:", WindowColour::GRAY);
		Utils::Console::Output(Utils::ProcessArchitecture(Memory::hProcess), WindowColour::WHITE);

		Utils::Console::Output("Process ID:", WindowColour::GRAY);
		Utils::Console::Output(String::to_string(Memory::dwPID), WindowColour::WHITE);

		Utils::Console::Output("------------------------------", WindowColour::GREEN);
	}
	else {
		Utils::Console::Output("Cannot find process!", WindowColour::GRAY);
	}

	
	DWORD zoomFunc = Memory::GetOffset("swtor.exe", "\xF3\x0F\x11\x81\xD0\x02\x00\x00\x5D", "xxxxxxxxx", 9, false);
	printf("%X", zoomFunc);
	//sprintf("0x%I64X\n", );
	
	MODULEENTRY32 client = Memory::GetModuleInformations("client.dll");

	//DWORD m_flFlashMaxAlpha = Memory::GetOffset("client.dll", "\x0F\x2F\xF2\x0F\x87\x00\x00\x00\x00\xF3\x0F\x10\xA1\x00\x00\x00\x00\x0F\x2F\xCC\x0F\x83", "xxxxx????xxxx????xxxxx", 13, false);
	// Memory::WriteMemory<bool>(Entity::GetEntityBaseAddress(3) + Offsets::m_bSpotted, 1)
	getchar();

    return 0;
}

