#include "Debug.h"

using namespace rav;

Debug::Debug()
{
	//Get STD Handlers and ScreenBuffer Information
	hstdin = GetStdHandle(STD_INPUT_HANDLE);
	hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	index = 0;
	GetConsoleScreenBufferInfo(hstdout, &csbi);

	SetConsoleColor(Black, Light_Yellow);
	std::cout << "\n DEBUG INITIALIZED \n\n";
	SetConsoleColor();
}

Debug::~Debug()
{

}

void rav::Debug::SetConsoleColor(ColorCodes foreground, ColorCodes background)
{
	SetConsoleTextAttribute(hstdout, static_cast<WORD>(((background & 0xff) << 4) + (foreground & 0xff)));
}

Debug & Debug::getDebug()
{
	static Debug deb;

	return deb;
}

void rav::Debug::PrintState()
{
	for (std::map<cstr, cstr>::iterator it = variables.begin(); it != variables.end(); it++)
	{
		std::cout << it->first << " => " << it->second << std::endl;
	}
}

void rav::Debug::Log(const std::string& message, MessageType type)
{
	//Write message prefix according to message type
	switch (type)
	{
	default:
	{
		SetConsoleColor(Bright_White);
		std::cout << "Log:";
		SetConsoleColor();
		std::cout << " ";
	}
	break;

	case rav::Debug::Variable:
	{
		SetConsoleColor(Bright_White, Light_Blue);
		std::cout << "Var:";
		SetConsoleColor();
		std::cout << " ";
	}
	break;

	case rav::Debug::Warning:
	{
		SetConsoleColor(Light_Yellow, Yellow);
		std::cout << "Warning:";
		SetConsoleColor(Black, Yellow);
		std::cout << " ";
	}
	break;

	case rav::Debug::Error:
	{
		SetConsoleColor(Light_Yellow, Red);
		std::cout << "Error:";
		SetConsoleColor(Bright_White, Red);
		std::cout << " ";
	}
	break;
	}

	//Output message
	std::cout << message << std::endl;

	//Reset Console Color
	SetConsoleColor();
}
