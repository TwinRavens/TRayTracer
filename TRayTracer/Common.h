#pragma once

//Common includes
#include <string>
#include <math.h>

//Custom includes
#include "Math.h"

//Namespace shortcut
using std::string;

//Typedefs
typedef const char * cstr;

//Common functions
static std::string hexify(unsigned int n)
{
	//Turn hex to string
	std::string res;
	do
	{
		res += "0123456789ABCDEF"[n % 16];
		n >>= 4;
	} while (n);

	return std::string(res.rbegin(), res.rend());
}