#pragma once

//OpenGL Includes
#include <GL\glew.h>

//Custom Includes
#include "Common.h"
#include "Debug.h"

//Defines
#define RV_VERTEX_SHADER			GL_VERTEX_SHADER
#define RV_FRAGMENT_SHADER			GL_FRAGMENT_SHADER
#define RV_GEOMETRY_SHADER			GL_GEOMETRY_SHADER
#define RV_TESS_CONTROL_SHADER		GL_TESS_CONTROL_SHADER
#define RV_TESS_EVALUATION_SHADER	GL_TESS_EVALUATION_SHADER

namespace rav
{
	class Shader
	{
	public:
		//Constructor with given shader definitions
		Shader(const string& shader_data, GLuint shader_type, const string& shader_name);

		//Destructor with OpenGL cleansing
		~Shader();

		//Compile Shader
		bool Compile();

		//Check compilation status
		bool isCompiled() const;

		//Get Shader's OpenGL ID
		GLuint getID() const;

		//Get Shader's Type (Vertex or Fragment)
		GLuint getType() const;

		//Set and get of shader name
		string getName() const;
		void setName(const string& shader_name);

	private:
		//Const char array with Shader ASCII Data
		string data;

		//Shader name if any
		string name;

		//Compilation flag
		bool compiled;

		//Type of shader
		GLuint type;

		//OpenGL Shader Index
		GLuint ID;
	};
}