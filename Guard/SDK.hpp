#pragma once

#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>
#include "Defines.hpp"
#include <string>
#include <sstream>

using namespace std;

namespace String
{
	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
}
