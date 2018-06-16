#pragma once
//std
#include <string>

//assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

//objects
#include "ObjectData.h"


static class RayFileLoader
{
private:
	RayFileLoader();
	~RayFileLoader();

	static ObjectData * InterpretMesh(aiMesh*mesh);
public:

	static ObjectData * LoadObject(const std::string& path);
};

