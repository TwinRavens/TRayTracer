#pragma once

//STD Includes
#include <string>
#include <fstream>
#include <streambuf>

//Library Includes
#include <ASSIMP\Importer.hpp>

//Ravine Includes
#include "Debug.h"
#include "Mesh.h"

//Using directives
using namespace std;

//Defines
#define rvLoadFile rav::FileLoader::Load

//This class is a fast text file loader
namespace rav
{
	static class FileLoader
	{
	public:
		//Loads the entire file in the passed string
		//Return the size of the string loaded, zero or less is an error code
		static int Load(const string& path, string& output, bool binary_mode = false);

		//Loads the entire file in the passed char array
		//Returns the size of the array loaded, zero or less is an error code
		static int Load(const string& path, char*& output, bool binary_mode = false);

		//Loads a mesh file and fills mesh reference
		static bool LoadMesh(const string& path, Mesh& mesh);
	};
}