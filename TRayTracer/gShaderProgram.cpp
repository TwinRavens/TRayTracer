#include "gShaderProgram.h"

//Namespaces for less typing :3
using namespace rav;
using namespace std;

rav::ShaderProgram::ShaderProgram(const string & program_name) : name(program_name), linked(false)
{
	//Ask for OpenGL to create a shader ID of that Shader Type
	ID = glCreateProgram();

	//Assert if no name
	_ASSERT(!program_name.empty());
}

rav::ShaderProgram::~ShaderProgram()
{
	//Detach all shaders
	multimap<GLuint, GLuint>::iterator shader_it;
	for (shader_it = attachments.begin(); shader_it != attachments.end(); shader_it++)
	{
		glDetachShader(ID, shader_it->second);
	}

	//Ask OpenGL to delete this program
	glDeleteProgram(ID);
};

bool rav::ShaderProgram::Attach(const GLuint & shader_id, const GLuint & shader_type, bool force_replace)
{
	//Log info
	rvDebug.Log("Attaching shader with ID " + std::to_string(shader_id) + " to program " + name + "...");

	//Check if there is a shader already attached
	if (isAttached(shader_id, shader_type))
	{
		if (force_replace)
		{
			//Throw warning
			rvDebug.Log("Shader with ID " + to_string(shader_id) + " already attached! Force replace enabled, replacing...\n", RV_WARNING_MESSAGE);

			//Detach existing shader
			glDetachShader(ID, shader_id);
		}
		else
		{
			//Throw warning
			rvDebug.Log("Shader with ID " + to_string(shader_id) + " already attached! Ignoring...\n", RV_WARNING_MESSAGE);

			//Return error code
			return false;
		}
	}

	//Attach new shader to program
	glAttachShader(ID, shader_id);

	//Log info
	rvDebug.Log("Successfully Attached!\n");

	//Return sucess code
	return true;
}

bool rav::ShaderProgram::Detach(const GLuint & shader_id, const GLuint & shader_type)
{
	if (isAttached(shader_id, shader_type))
	{
		//Throw Log
		rvDebug.Log("Detaching shader with ID " + to_string(shader_id) + " from " + name + "...");

		//Detach existing shader
		glDetachShader(ID, shader_id);

		//Throw Log
		rvDebug.Log("Detached with success!");

		//Return Sucess
		return true;
	}

	//Throw Warning
	rvDebug.Log("Shader with ID " + to_string(shader_id) + " is not attached!\n", RV_WARNING_MESSAGE);

	//Return error
	return false;
}

bool rav::ShaderProgram::isAttached(const GLuint& shader_id, const GLuint& shader_type) const
{
	//Get first pair of given shader_type
	multimap<GLuint, GLuint>::const_iterator it = attachments.find(shader_type);

	//Go through every shader of that type and make sure it have not been attached already
	for (it; it != attachments.end() && it->first == shader_type; it++)
	{
		//Is the shader in the list
		if (it->second == shader_id)
		{
			return true;
		}
	}

	//Return not attached
	return false;
}

bool rav::ShaderProgram::addAttributeLocation(const string &att_name)
{
	rvDebug.Log("Registering attribute " + att_name + " to shader " + name + "...");

	//Check if it is already linked
	if (!linked)
	{
		//Log Error
		rvDebug.Log("Shader " + name + " is not linked! You must link it first!\n", RV_ERROR_MESSAGE);

		//Return error code
		return false;
	}

	//Ask for OpenGL what is "vertex_position"'s attribute location
	GLint attLoc = glGetAttribLocation(ID, att_name.c_str());

	//Check if it's not an error (attLoc == -1)
	if (attLoc == -1)
	{
		//Log Error
		rvDebug.Log("Could not find attribute " + att_name + " in the vertex shader " + name + "!\n", RV_ERROR_MESSAGE);

		//Return error code
		return false;
	}

	//Register ID
	attributes.emplace(att_name, attLoc);

	//Log Sucess
	rvDebug.Log("Attribute " + att_name + " registered successfully!\n");

	//Return success
	return true;
}

bool rav::ShaderProgram::setAttributeLocation(const string& att_name, const GLint& att_loc)
{
	rvDebug.Log("Binding attribute " + att_name + " to program " + name + " with location " + to_string(att_loc) + "...");

	//Check if it is already linked
	if (linked)
	{
		//Log Error
		rvDebug.Log("Program " + name + " is already linked! You must re-link it AFTER setting an attribute location!\n", RV_WARNING_MESSAGE);
	}

	//Bind attribute location to given attribute variable (att_name)
	glBindAttribLocation(ID, att_loc, att_name.c_str());

	//Get iterator
	map<string, GLint>::const_iterator it = attributes.find(att_name);

	//Register ID
	attributes.emplace(att_name, att_loc);

	//Log Sucess
	rvDebug.Log("Attribute " + att_name + " binded successfully!\n");

	//Return success
	return true;
}

GLuint rav::ShaderProgram::getAttributes(ShaderAttribute*& container) const
{
	//Allocate memory
	container = new ShaderAttribute[attributes.size()];

	//Fill container entries
	int i = 0;
	map<string, GLint>::const_iterator it;
	for (it = attributes.cbegin(); it != attributes.end(); it++)
	{
		container[i].name = (it->first);
		container[i].location = (it->second);
		i++;
	}

	//Return it's size
	return attributes.size();
}

GLint rav::ShaderProgram::getAttribute(const string & att_name) const
{
	//Get iterator
	map<string, GLint>::const_iterator it = attributes.find(att_name);

	//If no attribute was found
	if (it == attributes.end())
	{
		//Log Error
		rvDebug.Log("Attribute " + att_name + " in program " + name + " was not found!\n", RV_ERROR_MESSAGE);

		//Return error code
		return -1;
	}

	//Return attribute location
	return it->second;
}

bool rav::ShaderProgram::remAttribute(const string& att_name)
{
	//Log info
	rvDebug.Log("Removing attribute " + att_name + " ...");

	//Get attribute ID
	GLint id = getAttribute(att_name);

	//If ID not valid
	if (id < 0)
		return false; //Return error code

	//Remove attribute from list
	attributes.erase(att_name);
}

void rav::ShaderProgram::Use()
{
	//Use this program
	glUseProgram(ID);
}

bool rav::ShaderProgram::Link()
{
	//Point out that we are Linking
	rvDebug.Log("Linking program " + name + "...");

	//Tell OpenGL to Link it
	glLinkProgram(ID);

	//Get Linking Result Code
	GLint isLinked = 0;
	glGetProgramiv(ID, GL_LINK_STATUS, &isLinked);

	//If got an error
	if (isLinked == GL_FALSE)
	{
		//Get size of info log
		GLint maxLength = 0;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		GLchar *errorLog = new GLchar[maxLength];
		glGetProgramInfoLog(ID, maxLength, &maxLength, &errorLog[0]);

		//Error header
		rvDebug.Log("Error compiling Shader " + name, Debug::Error);

		//Create message
		string msg((const char*)errorLog);

		//Pop last \0 char
		msg.pop_back();

		//Log Error through Debugger
		rvDebug.Log(msg, Debug::Error);

		//Don't leak the shader
		glDeleteProgram(ID);

		//Exit with failure.
		return (linked = false);
	}

	//Log sucesfull compilation message
	rvDebug.Log("Successfully Linked!\n");

	//No compilation error! Compilation succeded!
	return (linked = true);
}

bool rav::ShaderProgram::isLinked() const
{
	return linked;
}

GLuint rav::ShaderProgram::getID() const
{
	return ID;
}

string rav::ShaderProgram::getName() const
{
	return name;
}

void rav::ShaderProgram::setName(const string& program_name)
{
	name = program_name;
}
