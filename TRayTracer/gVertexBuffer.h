#pragma once

//STD Includes
#include <list>
#include <string>

//Custom Includes
#include "Common.h"
#include "Debug.h"

//Using shortcuts
using std::list;
using std::string;

namespace rav
{
	//Struct that defines a vertex buffer data set (will be available for
    //one vertex at the time and describs a repeating portion of the buffer)
	//One or more structs must be set to describe how data will be feed
	//into the shader pipeline. The same buffer CAN have more than one
	//descriptors as long as the attribute location for the data differs
	struct VertexBufferDescriptor
	{
		VertexBufferDescriptor(GLint attribute_location, GLint data_amount, GLenum data_type)
		{
			this->attribute_location = attribute_location;
			this->data_amount = data_amount;
			this->data_type = data_type;
			this->normalized = GL_FALSE;
			this->stride = 0;
			this->pointer = NULL;
		}

		VertexBufferDescriptor(GLint attribute_location, GLint data_amount, GLenum data_type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
		{
			this->attribute_location = attribute_location;
			this->data_amount = data_amount;
			this->data_type = data_type;
			this->normalized = normalized;
			this->stride = stride;
			this->pointer = pointer;
		}

		GLint attribute_location;
		GLint data_amount;
		GLenum data_type;
		GLboolean normalized;
		GLsizei stride;
		const GLvoid* pointer;
	};

	//This class manages vertex buffer data allocated in OpenGL's memory
	//and the respective data description for access via shader attribute
	//location. 
	class VertexBuffer
	{
	protected:
		//List of buffer descriptors, should be only one if no interlaced
		//data is used. Still it's more optimized to do interlaced data!
		list<VertexBufferDescriptor> *buffer_descriptors;

		//OpenGL's given Vertex Buffer Object ID
		GLuint ID;

		//BufferData has been feed to OpenGL
		bool filled;

		//Amount of data objects
		size_t buffer_size;

		//Size of one instance of the data structure defined by the descriptors
		//Aka. One Face (triangle/etc)
		size_t buffer_structure_size;

	public:
		//Default Constructor and Destructor
		VertexBuffer();
		~VertexBuffer();

		//Constructors with descriptor data
		VertexBuffer(VertexBufferDescriptor buffer_descriptor);

		//Additional Descriptor operations
		bool AddBufferDescriptor(VertexBufferDescriptor buffer_descriptor);						//Add buffer descriptor, attribute locations are unique
		bool GetBufferDescriptor(GLint attribute_location, VertexBufferDescriptor& containter); //Get buffer descriptor and fill given container
		bool RemoveBufferDescriptor(GLint attribute_location);									//Remove descriptor with such attribute location

		//Get current Vertex Buffer ID (a zero return means OpenGL's memory has been freed)
		GLuint getID() const;

		//Bind current VertexBuffer (creates a new name ID if none is set)
		void Bind();
        void Bind(const GLenum buffer_target);

		//Unbind current VertexBuffer
		static void Unbind();
        static void Unbind(const GLenum buffer_target);

		//Fill OpenGL's memory with given buffer data
		bool Fill(const GLsizeiptr buffer_size, const GLvoid * data_ptr);
		bool Fill(const GLsizeiptr buffer_size, const GLvoid * data_ptr, const GLenum usage_type);
        bool Fill(const GLenum buffer_target, const GLsizeiptr buffer_size, const GLvoid * data_ptr, const GLenum usage_type);

		//Clear OpenGL's memory for given buffer data
		void Free();

		//Set VertexBuffer attribute pointers as defined by it's descriptors
		bool SetAttributePointers();

		//Draw VertexBuffer Array ("Bind()" is automatically called over here)
		void Draw();
        void Draw(GLenum geomtry_type);

		//Draw a section of the VertexBuffer Array ("Bind()" is automatically called over here)
		void Draw(size_t start_index, size_t count);
        void Draw(GLenum geometry_type, size_t start_index, size_t count);
	};
}