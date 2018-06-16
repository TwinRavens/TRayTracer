#include "RayFileLoader.h"
#include "Debug.h"


RayFileLoader::RayFileLoader()
{
}


RayFileLoader::~RayFileLoader()
{
}

ObjectData* RayFileLoader::InterpretMesh(aiMesh * mesh)
{
	if (mesh == nullptr) {
		rvDebug.Log("Mesh is null, something went wrong", rvDebug.Error);
		return nullptr;
	}
	ObjectData *data = new ObjectData();
	int count = data->verticesCount = mesh->mNumVertices;
	data->vertices = new glm::vec3[count];
	data->normal = new glm::vec3[count];
	for (int i = 0; i < count; i++)
	{
		data->vertices[i] = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		data->normal[i] = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		//missing triangle
	}
	return data;
}

ObjectData* RayFileLoader::LoadObject(const std::string& path)
{
#pragma region LoadAssimp
	// Create an instance of the Importer class
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenNormals
	);

	// If the import failed, report it
	if (!scene)
	{
		rvDebug.Log(importer.GetErrorString(), rav::Debug::Error);
		return nullptr;
	}
	if (!scene->HasMeshes() && scene->mNumMeshes == 0)
	{
		rvDebug.Log("No mesh on file", rav::Debug::Error);
		return nullptr;
	}
#pragma endregion

	if (scene->mNumMeshes == 1)
	{
		return InterpretMesh(scene->mMeshes[0]);
	}
	else
	{
		ObjectData* finalObject = new ObjectData();
		//create a holder for every object data
		ObjectData** temp = new ObjectData*[scene->mNumMeshes];

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			temp[i] = InterpretMesh(scene->mMeshes[i]);
			finalObject->verticesCount += temp[i]->verticesCount;
		}

		finalObject->vertices = new glm::vec3[finalObject->verticesCount];
		finalObject->normal = new glm::vec3[finalObject->verticesCount];

		//get all of the objectData together
		for (int i = 0, j = 0; i < scene->mNumMeshes; i++)
		{
			for (int k = 0; k < temp[i]->verticesCount; k++, j++)
			{
				finalObject->vertices[j] = temp[i]->vertices[k];
				finalObject->normal[j] = temp[i]->normal[k];
			}
			delete temp[i];
		}

		return finalObject;
	}

}
