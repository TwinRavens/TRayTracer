#include "gShader.h"

rav::Shader::Shader(const string & shader_data, GLuint shader_type, const string & shader_name) : data(shader_data), type(shader_type), name(shader_name), compiled(false)
{
	//Ask for OpenGL to create a shader ID of that Shader Type
	ID = glCreateShader(shader_type);

	//Assert if no name or data empty
	_ASSERT(!shader_name.empty() && !shader_data.empty());
}

rav::Shader::~Shader()
{
	//Ask for OpenGL to delete that shader
	glDeleteShader(ID);
};

bool rav::Shader::Compile()
{
	//Get const char from string data
	cstr cdata = data.c_str();

	//Set source text for shader data
	glShaderSource(ID, 1, &cdata, NULL);

	//Point out that we are compiling
	rvDebug.Log("Compiling Shader " + name);

	//Tell OpenGL to compile it
	glCompileShader(ID);

	//Get Compilation Result Code
	GLint isCompiled = 0;
	glGetShaderiv(ID, GL_COMPILE_STATUS, &isCompiled);

	//If got an error
	if (isCompiled == GL_FALSE)
	{
		//Get size of info log
		GLint maxLength = 0;
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		GLchar *errorLog = new GLchar[maxLength];
		glGetShaderInfoLog(ID, maxLength, &maxLength, &errorLog[0]);

		//Error header
		rvDebug.Log("Error compiling Shader " + name, Debug::Error);

		//Create message
		string msg((const char*)errorLog);

		//Pop last \0 char
		msg.pop_back();
		
		//Log Error through Debugger
		rvDebug.Log(msg, Debug::Error);

		//Don't leak the shader
		glDeleteShader(ID);

		//Exit with failure.
		return (compiled = false);
	}

	//Log sucesfull compilation message
	rvDebug.Log("Sucessfully compiled!\n");

	//No compilation error! Compilation succeded!
	return (compiled = true);
}

bool rav::Shader::isCompiled() const
{
	//Return compiled flag
	return compiled;
}

GLuint rav::Shader::getID() const
{
	return ID;
}

unsigned int rav::Shader::getType() const
{
	return (unsigned int)type;
}

string rav::Shader::getName() const
{
	return name;
}

void rav::Shader::setName(const string& shader_name)
{
	name = shader_name;
}

