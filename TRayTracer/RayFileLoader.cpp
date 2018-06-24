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
	// load vertices
	int count = data->verticesCount = mesh->mNumVertices;
	data->vertices = new glm::vec4[count];
	data->normal = new glm::vec4[count];
	for (int i = 0; i < count; i++)
	{
		data->vertices[i] = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 0);
		data->normal[i] = glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 0);
	}

	count = data->triangleCount = mesh->mNumFaces;
	data->triangles = new Triangle[count];
	for (int i = 0; i < count; i++)
	{

		data->triangles[i] = Triangle{ mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2], 1,
										mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2], 0 };
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
		ObjectData* obj = InterpretMesh(scene->mMeshes[0]);
		rvDebug.Log("Loaded " + path + " sucessfully. It has " + std::to_string(obj->verticesCount) + " vertices and " + std::to_string(obj->triangleCount) + " triangles");
		return obj;
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
			finalObject->triangleCount += temp[i]->triangleCount;
		}

		finalObject->vertices = new glm::vec4[finalObject->verticesCount];
		finalObject->normal = new glm::vec4[finalObject->verticesCount];
		finalObject->triangles = new Triangle[finalObject->triangleCount];

		//get all of the objectData together
		int cIndex = 0;
		for (int i = 0, cVert = 0, cTri = 0; i < scene->mNumMeshes; i++)
		{
			for (int k = 0; k < temp[i]->verticesCount; k++, cVert++)
			{
				finalObject->vertices[cVert] = temp[i]->vertices[k];
				finalObject->normal[cVert] = temp[i]->normal[k];
			}
			for (int k = 0; k < temp[i]->triangleCount; k++, cTri++)
			{
				finalObject->triangles[cTri] = {	temp[i]->triangles[k].v1 + cIndex, temp[i]->triangles[k].v2 + cIndex, temp[i]->triangles[k].v3 + cIndex, temp[i]->triangles[k]._padding1,
													temp[i]->triangles[k].n1 + cIndex, temp[i]->triangles[k].n2 + cIndex, temp[i]->triangles[k].n3 + cIndex, temp[i]->triangles[k]._padding2 };
			}
			cIndex += temp[i]->verticesCount;
			delete temp[i];
		}

		rvDebug.Log("Loaded " + path + " sucessfully. It has " + std::to_string(finalObject->verticesCount) + " vertices and " + std::to_string(finalObject->triangleCount) + " triangles");

		return finalObject;
	}

}
