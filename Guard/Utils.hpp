#pragma once

#include "SDK.hpp"
#include <iostream>
#include <Psapi.h>
#include <tchar.h>

enum WindowColour
{
	DARKBLUE = 1,
	DARKGREEN,
	DARKTEAL,
	DARKRED,
	DARKPINK,
	DARKYELLOW,
	GRAY,
	DARKGRAY,
	BLUE,
	GREEN,
	TEAL,
	RED,
	PINK,
	YELLOW,
	WHITE
};

namespace Utils
{
	namespace Console {
		void Spawn(const char* title = nullptr);
		void Output(string text);
		void Output(string text, WindowColour PreColor, WindowColour PostColor = WindowColour::WHITE);
	}

	int Random(int min, int max);
	BOOL Is64Bit(HANDLE hProcess);
	char* ProcessArchitecture(HANDLE hProcess);
}