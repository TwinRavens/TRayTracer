#pragma once

//STD Includes
#include <map>

//OpenGL Includes
#include <GL\glew.h>

//Custom Includes
#include "Common.h"
#include "Debug.h"
#include "gShader.h"
#include "gShaderAttribute.h"

//Using Shortcuts
using std::multimap;
using std::map;

//Defines
#define RV_VERTEX_POS_ATT	"vertex_position"
#define RV_VERTEX_COL_ATT	"vertex_color"
#define RV_VERTEX_UV_ATT	"texture_coordinates"

namespace rav
{
	class ShaderProgram
	{
	protected:
		//Multi map for every shader type with shader ids
		multimap<GLuint, GLuint> attachments;

		//Map for every attribute ID with it's name
		map<string, GLint> attributes;

		//Program name
		string name;

		//Linking flag
		bool linked;

		//OpenGL Program Index
		GLuint ID;

	public:
		//Constructor with program name definition
		ShaderProgram(const string& program_name);

		//Destructor with OpenGL cleansing
		~ShaderProgram();

		//Attach Shader to this program
		bool Attach(const GLuint& shader_id, const GLuint& shader_type, bool force_replace = false);

		//Detach Shader to this program
		bool Detach(const GLuint& shader_id, const GLuint& shader_type);

		//Tells whether or not a given shader is attached
		bool isAttached(const GLuint& shader_id, const GLuint& shader_type) const;

		//Find an attribute in the shaders and add it's location to the attributes list
		//ONLY AFTER LINKING
		bool addAttributeLocation(const string& att_name);

		//Assign an attribute in the shaders, set it's location and add it to the attributes list
		//ONLY BEFOR LINKING AND MUST LINK TO APPLY CHANGES
		bool setAttributeLocation(const string& att_name, const GLint& att_loc);

		//Gets attributes list, fill the container with a copy and return it's size
		GLuint getAttributes(ShaderAttribute*& container) const;

		//Gets attribute from list and return it's location
		GLint getAttribute(const string& att_name) const;

		//Remove an attribute from this shader
		bool remAttribute(const string& att_name);

		//Use program
		void Use();

		//Link program
		bool Link();

		//Check compilation status
		bool isLinked() const;

		//Get Shader's OpenGL ID
		GLuint getID() const;

		//Set and get of shader name
		string getName() const;
		void setName(const string& program_name);
	};
}