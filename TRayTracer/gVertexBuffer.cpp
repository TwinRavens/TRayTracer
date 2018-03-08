#include "gVertexBuffer.h"

rav::VertexBuffer::VertexBuffer()
{
	//Ask OpenGL for a Vertex Buffer Object ID
	glGenBuffers(1, &ID);

	//Default variable values
	filled = false;

	//Instantiate descriptors list
	buffer_descriptors = new list<VertexBufferDescriptor>();
}

rav::VertexBuffer::~VertexBuffer()
{
	//Clear buffer_descriptors list
	delete buffer_descriptors;

	//Free any allocated GL's memory
	Free();
}

rav::VertexBuffer::VertexBuffer(VertexBufferDescriptor buffer_descriptor)
{
	//Ask OpenGL for a Vertex Buffer Object ID
	glGenBuffers(1, &ID);

	//Default variable values
	filled = false;

	//Instantiate descriptors list
	buffer_descriptors = new list<VertexBufferDescriptor>();

	//Add given buffer descriptor
	buffer_descriptors->push_back(buffer_descriptor);
}

bool rav::VertexBuffer::AddBufferDescriptor(VertexBufferDescriptor buffer_descriptor)
{
	//Check if there is a buffer descriptor with such location yet
	list<VertexBufferDescriptor>::iterator it;
	for (it = buffer_descriptors->begin(); it != buffer_descriptors->end(); it++)
	{
		//If another descriptor already is defined in that location
		if (it->attribute_location == buffer_descriptor.attribute_location)
			return false; //Return error
	}

	//Add buffer descriptor to list
	buffer_descriptors->push_back(buffer_descriptor);

	//As the buffer structure description has changed,
	//update structure size to do the drawing right!
	size_t description_size = buffer_descriptor.data_amount;
	switch (buffer_descriptor.data_type)
	{
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		description_size *= sizeof(char);
		break;

	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
	case GL_HALF_FLOAT:
		description_size *= sizeof(char) * 2;
		break;

	case GL_INT:
	case GL_UNSIGNED_INT:
	case GL_FIXED:
	case GL_FLOAT:
		description_size *= sizeof(char) * 4;
		break;

	case GL_DOUBLE:
		description_size *= sizeof(char) * 8;
		break;
	}
	buffer_structure_size += description_size;

	//Return sucess
	return true;
}

bool rav::VertexBuffer::GetBufferDescriptor(GLint attribute_location, VertexBufferDescriptor& containter)
{
	//Check if there is a buffer descriptor with such location
	list<VertexBufferDescriptor>::iterator it;
	for (it = buffer_descriptors->begin(); it != buffer_descriptors->end(); it++)
	{
		//If a descriptor is defined in that location
		if (it->attribute_location == attribute_location)
		{
			containter = (*it);
			return true; //Return true
		}
	}

	//None found, return error
	return false;
}

bool rav::VertexBuffer::RemoveBufferDescriptor(GLint attribute_location)
{
	//Check if there is a buffer descriptor with such location
	list<VertexBufferDescriptor>::iterator it;
	for (it = buffer_descriptors->begin(); it != buffer_descriptors->end(); it++)
	{
		//If a descriptor is defined in that location
		if (it->attribute_location == attribute_location)
		{
			//As the buffer structure description has changed,
			//update structure size to do the drawing right!
			size_t description_size = it->data_amount;
			switch (it->data_type)
			{
			case GL_BYTE:
			case GL_UNSIGNED_BYTE:
				description_size *= sizeof(char);
				break;

			case GL_SHORT:
			case GL_UNSIGNED_SHORT:
			case GL_HALF_FLOAT:
				description_size *= sizeof(char) * 2;
				break;

			case GL_INT:
			case GL_UNSIGNED_INT:
			case GL_FIXED:
			case GL_FLOAT:
				description_size *= sizeof(char) * 4;
				break;

			case GL_DOUBLE:
				description_size *= sizeof(char) * 8;
				break;
			}
			buffer_structure_size -= description_size;

			//Buffer descriptor found, errasing it
			buffer_descriptors->erase(it);

			//Return sucess
			return true;
		}
	}

	//Not found
	return false;
}

GLuint rav::VertexBuffer::getID() const
{
	return ID;
}

void rav::VertexBuffer::Bind()
{
    Bind(GL_ARRAY_BUFFER);
}

void rav::VertexBuffer::Bind(GLenum buffer_target)
{
    //If ID is invalid
    if (ID == 0)
        glGenBuffers(1, &ID); //Ask OpenGL for a new one!

    //Bind such Vertex Buffer
    glBindBuffer(buffer_target, ID);
}

void rav::VertexBuffer::Unbind()
{
    Unbind(GL_ARRAY_BUFFER);
}

void rav::VertexBuffer::Unbind(const GLenum buffer_target)
{
    //Unbind any current bound vertex buffer array
    glBindBuffer(buffer_target, 0);
}

bool rav::VertexBuffer::Fill(GLsizeiptr buffer_size, const GLvoid * data_ptr)
{
	return Fill(GL_ARRAY_BUFFER, buffer_size, data_ptr, GL_STATIC_DRAW);
}

bool rav::VertexBuffer::Fill(GLsizeiptr buffer_size, const GLvoid * data_ptr, const GLenum usage_type)
{
    return Fill(GL_ARRAY_BUFFER, buffer_size, data_ptr, usage_type);
}

bool rav::VertexBuffer::Fill(const GLenum buffer_target, const GLsizeiptr buffer_size, const GLvoid * data_ptr, const GLenum usage_type)
{
    //Bind current Vertex Buffer
    Bind();

    //If buffer size is invalid return error
    if (buffer_size < 1)
        return false;

    //Copy data to OpenGL's memory
    glBufferData(buffer_target, buffer_size, data_ptr, usage_type);

    //Hold data_count value
    this->buffer_size = buffer_size;

    //Flag as filled
    filled = true;

    //Return sucess
    return true;
}

void rav::VertexBuffer::Free()
{
	//Delete current VertexBuffer data
	glDeleteBuffers(1, &ID);

	//Unflag filled
	filled = false;
}

bool rav::VertexBuffer::SetAttributePointers()
{
	//If ID not valid or data not filled return error
	if (ID == 0 || !filled)
		return false;

	//Bind this VertexObject
	Bind();

	//Use data as defined by each descriptor
	list<VertexBufferDescriptor>::iterator it;
	for (it = buffer_descriptors->begin(); it != buffer_descriptors->end(); it++)
	{
		glVertexAttribPointer(it->attribute_location, it->data_amount, it->data_type, it->normalized, it->stride, it->pointer);
	}

	//Return success
	return true;
}

void rav::VertexBuffer::Draw()
{
    Draw(GL_TRIANGLES);
}

void rav::VertexBuffer::Draw(GLenum geomtry_type)
{
    //Trow error when trying to draw unfilled array
    if (!filled)
    {
        rvDebug.Log("Trying to draw unfilled array of ID" + std::to_string(ID) + "!", RV_ERROR_MESSAGE);
        return;
    }

    //Bind this vertex buffer
    Bind();

    //Draw data array
    Draw(geomtry_type, 0, buffer_size / buffer_structure_size);
}

void rav::VertexBuffer::Draw(size_t start_index, size_t count)
{
	Draw(GL_TRIANGLES, start_index, count);
}

void rav::VertexBuffer::Draw(GLenum geometry_type, size_t start_index, size_t count)
{
    //Bind this buffer
    Bind();

    //Draw data array
    glDrawArrays(geometry_type, start_index, count);
}

