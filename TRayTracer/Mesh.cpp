#include "Mesh.h"

rav::Mesh::Mesh()
{
}

rav::Mesh::~Mesh()
{
	//Clear mesh buffer
	delete data;

	//Unload mesh data from OpenGL
	vbo.Free();
}

rav::Mesh::Mesh(char* mesh_data, size_t data_size, const VertexBufferDescriptor &descriptor, GLenum usage_type, GLenum geometry_type) : geometry_type(geometry_type)
{
	//Hold a copy of the buffer
	memcpy(this->data, mesh_data, data_size);

	//Copy data to OpenGL
	vbo.Fill(data_size, mesh_data, usage_type);

	//Add descriptor to VBO
	vbo.AddBufferDescriptor(descriptor);

	//Copy data to OpenGL
	PushMesh();
}

rav::Mesh::Mesh(char* mesh_data, size_t data_size, const VertexBufferDescriptor * descriptor, int size, GLenum usage_type, GLenum geometry_type) : geometry_type(geometry_type), usage_type(usage_type)
{
	//Hold a copy of the buffer
	memcpy(this->data, mesh_data, data_size);

	//Copy data to OpenGL
	vbo.Fill(data_size, mesh_data, usage_type);

	//Iterate through every descriptor and add it to the VBO
	for (int i = 0; i < size; i++)
	{
		vbo.AddBufferDescriptor(descriptor[i]);
	}

	//Copy data to OpenGL
	PushMesh();
}

size_t rav::Mesh::GetMesh(char *& data_output)
{
	if (data_size > 0)
	{
		//Allocate memory for the external buffer
		data_output = new char[data_size];

		//Copy all the local data to the external buffer
		memcpy(data_output, data, data_size);
	}

	//Return it's size
	return data_size;
}

size_t* rav::Mesh::GetMeshPtr(char*& data_ptr)
{
	//Point given ptr to local data ptr
	data_ptr = data;

	//Return the address of data_size
	return &data_size;
}

bool rav::Mesh::PushMesh()
{
	//Invalid size
	if (data_size <= 0)
		return false;

	//Push data to OpenGL
	vbo.Fill(data_size, data, usage_type);

	return true;
}

void rav::Mesh::SetMesh(char* mesh_data, size_t data_size, GLenum usage_type, GLenum geometry_type, bool late_push)
{
	//Hold a copy of the buffer
	memcpy(this->data, mesh_data, data_size);

	//Copy usage type and data size
	this->data_size = data_size;
	this->geometry_type = usage_type;
	this->usage_type = usage_type;

	//If must, push data to OpenGL
	if (!late_push)
	{
		//Copy data to OpenGL
		PushMesh();
	}
}

void rav::Mesh::AddDescriptors(const VertexBufferDescriptor * descriptors, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		vbo.AddBufferDescriptor(descriptors[i]);
	}
}

void rav::Mesh::Draw()
{
	//Tell VBO to draw to framebuffer
	vbo.Draw();
}
