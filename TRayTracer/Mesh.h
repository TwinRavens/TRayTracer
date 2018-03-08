#pragma once

//Custom Includes
#include "gVertexBuffer.h"

namespace rav
{
	//Mesh is a class that contains mesh data as well
	//as it's attributes interface setup (VBO)
	class Mesh
	{
	private:
		char* data;				//A copy of the mesh's data
		size_t data_size;		//Size of the mesh's data in bytes
		VertexBuffer vbo;		//The VBO that handles the OpenGL buffer
		GLenum usage_type;		//Type of usage for this buffer in memory
		GLenum geometry_type;	//Type of geometry this buffer represents

	public:
		//Default constructor for empty mesh
		Mesh();
		~Mesh();

		//Construct mesh with VBO buffer and one descriptor
		Mesh(char* mesh_data, size_t data_size, const VertexBufferDescriptor& descriptor, GLenum usage_type = GL_STATIC_DRAW, GLenum geometry_type = GL_STATIC_DRAW);

		//Construct mesh with VBO buffer and many descriptors
		Mesh(char* mesh_data, size_t data_size, const VertexBufferDescriptor* descriptor, int size, GLenum usage_type = GL_STATIC_DRAW, GLenum geometry_type = GL_TRIANGLES);

		//Fill output array of chars with mesh data and returns it's size
		size_t GetMesh(char*& data_output);

		//Redirect pointer to local mesh data, returns data size pointer
		size_t* GetMeshPtr(char*& data_ptr);
		
		//Override current mesh data (late push will not push data to OpenGL - do it with PushMesh() call)
		void SetMesh(char* mesh_data, size_t data_size, GLenum usage_type = GL_STATIC_DRAW, GLenum geometry_type = GL_TRIANGLES, bool late_push = false);

		//Add an array of descriptor to this mesh
		void AddDescriptors(const VertexBufferDescriptor* descriptors, int size);

		//Draw Mesh with Given Shader
		void Draw();

		//Pushes mesh data to OpenGL, returns true if succedded
		bool PushMesh();
	};
}


