#include "gVertexArray.h"

rav::VertexArray::VertexArray()
{
	//Ask OpenGL for an Array ID
	glGenVertexArrays(1, &ID);
}

rav::VertexArray::~VertexArray()
{
	//Free Vertex Array from OpenGL's memory
	glDeleteVertexArrays(1, &ID);

	//Set ID as zero, no-allocated
	ID = 0;
}

rav::VertexArray::VertexArray(const GLuint & program_id)
{
	//Ask OpenGL for an Array ID
	glGenVertexArrays(1, &ID);

	//Enable shader attributes' locations
	EnableLocations(program_id);
}

rav::VertexArray::VertexArray(const ShaderProgram & program)
{
	//Ask OpenGL for an Array ID
	glGenVertexArrays(1, &ID);

	//Enable shader attributes' locations
	EnableLocations(program);
}

void rav::VertexArray::EnableLocation(GLint attribute_location) const
{
	//Bind this Vertex Array
	Bind();

	//Enable it's location
	glEnableVertexAttribArray(attribute_location);
}

void rav::VertexArray::EnableLocations(const ShaderProgram & program) const
{
	//Bind this VertexArray
	Bind();

	//Get attributes from shader
	ShaderAttribute* attributes;
	int size = program.getAttributes(attributes);
	
	//Enable each location
	for (size_t i = 0; i < size; i++)
	{
		glEnableVertexAttribArray(attributes[i].location);
	}
}

void rav::VertexArray::EnableLocations(const GLuint & program_id) const
{
	//Get Shader Reference
	ShaderProgram *shp;

	//If reference found
	if (rvGetProgram(program_id, shp))
	{
		//Enable it's locations
		EnableLocations(*shp);
	}
}

void rav::VertexArray::EnableLocations(unsigned int size, int * locations) const
{
	//Bind this Vertex Array
	Bind();

	//For each entry in the array
	for (size_t i = 0; i < size; i++)
	{
		//Enable it's location
		glEnableVertexAttribArray(locations[i]);
	}
}

void rav::VertexArray::DisableLocation(GLint attribute_location) const
{
	//Bind this Vertex Array
	Bind();

	//Disable it's location
	glDisableVertexAttribArray(attribute_location);
}

GLuint rav::VertexArray::getID() const
{
	return ID;
}

void rav::VertexArray::Bind() const
{
	glBindVertexArray(ID);
}

void rav::VertexArray::Unbind()
{
	glBindVertexArray(0);
}
