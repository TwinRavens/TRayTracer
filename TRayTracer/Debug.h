#pragma once

//Std Include
#include <Windows.h>
#include <iostream>
#include <strstream>
#include <typeinfo>
#include <map>

//OpenGL Includes
#include <GL\glew.h>

//Custom includes
#include "Common.h"

//Defines
#define DEBUG_CONSOLE_MODE //Debug in Console Mode (OpenGL interface WIP)
#define rvDebug rav::Debug::getDebug()
#define RV_WARNING_MESSAGE rav::Debug::Warning
#define RV_ERROR_MESSAGE rav::Debug::Error
#define RV_COMMON_MESSAGE rav::Debug::Common
#define RV_VARIABLE_MESSAGE rav::Debug::Variable

namespace rav
{
#ifdef DEBUG_CONSOLE_MODE
	class Debug
	{
	private:
		//Private Constructors for singleton
		Debug();
		~Debug();

		//Console Color Codes
		enum ColorCodes
		{
			Black,
			Blue,
			Green,
			Aqua,
			Red,
			Purple,
			Yellow,
			White,
			Gray,
			Light_Blue,
			Light_Green,
			Light_Aqua,
			Light_Red,
			Light_Purple,
			Light_Yellow,
			Bright_White
		};

		//Mapping of variables (name, value)
		std::map<cstr, cstr> variables;

		//Set console color according to given foreground and background
		void SetConsoleColor(ColorCodes foreground = ColorCodes::White, ColorCodes background = ColorCodes::Black);

		//IO Handlers and Indexes
		HANDLE hstdin;
		HANDLE hstdout;
		WORD index;
		CONSOLE_SCREEN_BUFFER_INFO csbi;

	public:
		//Delete Override operations
		Debug(Debug const&) = delete;
		void operator=(Debug const&) = delete;

		//Static get for instancing
		static Debug& getDebug();

		//Message Type Enumerator
		enum MessageType
		{
			Common,
			Variable,
			Warning,
			Error
		};

		//Set a variable on the monitor (return variable data as str)
		template<class T>
		cstr SetVar(cstr var_name, T* var_value)
		{
			//Setup a stream for using << operator
			std::ostringstream ss;
			ss << (*var_value);

			//Get stream data
			std::string str = ss.str();

			//Register variable in map
			variables.emplace(var_name, str.c_str());
			
			//Return content
			return str;
		};

		//Print all seted variables
		void PrintState();

		//Throw message to console of given message type
		void Log(const std::string& message, MessageType type = MessageType::Common);
	};
#endif
}