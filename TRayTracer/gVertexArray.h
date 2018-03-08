#pragma once

//Custom Includes
#include "Common.h"
#include "Debug.h"
#include "gShaderProgram.h"
#include "gShaderManager.h"

namespace rav
{
	//A VertexArray holds the state of the current bound VertexBuffers,
	//their attributes and which attribute locations are enabled
	class VertexArray
	{
	private:
		GLuint ID;

	public:
		//Default constructor and destructor
		VertexArray();
		~VertexArray();

		//Initialize vertex array and enable every location from shader registered attributes
		VertexArray(const GLuint& program_id);

		//Initialize vertex array and enable every location from shader registered attributes
		VertexArray(const ShaderProgram& program);

		//Enables given attribute location (saves such information for each time
		//this VertexArray is bound to OpenGL's State Machine
		void EnableLocation(GLint attribute_location) const;

		//Enable all locations from shader registered attributes
		void EnableLocations(const GLuint& program_id) const;

		//Enable all locations from shader registered attributes
		void EnableLocations(const ShaderProgram& program) const;

		//Enable locations given an array of locations and it's size
		void EnableLocations(unsigned int size, int* locations) const;

		//Disable given attribute location (oposite of EnableLocation())
		void DisableLocation(GLint attribute_location) const;

		//Get ID
		GLuint getID() const;

		//By binding this VertexArray, the Array State is loaded into OpenGL's memory
		void Bind() const;

		//Unbinding reset state to default
		static void Unbind();
	};
}

