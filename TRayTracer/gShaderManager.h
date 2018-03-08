#pragma once

//Iostream for output
#include <iostream>

//Main Includes
#include <GL\glew.h>
#include <GLFW\glfw3.h>

//Custom Includes
#include "Common.h"
#include "Debug.h"
#include "gShader.h"
#include "gShaderProgram.h"

//Using Shortcuts
using std::map;

//Singleton Defines
#define rvShaderManager		rav::ShaderManager::get()

//Shader Defines
#define rvGetShader			rvShaderManager.getShader
#define rvAddShader			rvShaderManager.addShader
#define rvCreateShader		rvShaderManager.createShader
#define rvRemoveShader		rvShaderManager.removeShader

//Shader Program Defines
#define rvGetProgram		rvShaderManager.getProgram
#define rvAddProgram		rvShaderManager.addProgram
#define rvCreateProgram		rvShaderManager.createProgram
#define rvAttachShader		rvShaderManager.attachShader
#define rvDetachShader		rvShaderManager.detachShader
#define rvAddAttribute		rvShaderManager.addAttribute
#define rvRemAttribute		rvShaderManager.remAttribute
#define rvGetAttributeLoc	rvShaderManager.getAttributeLocation
#define rvSetAttributeLoc	rvShaderManager.setAttributeLocation
#define rvLinkProgram		rvShaderManager.linkProgram
#define rvUseProgram		rvShaderManager.useProgram
#define rvGetCurrentProgram rvShaderManager.getCurrentProgram

namespace rav
{
	class ShaderManager
	{
	private:
		//Private constructor for singleton
		ShaderManager();

		//And destructor
		~ShaderManager();

		//Program in use
		ShaderProgram* currentProgram;

		//Map of all registered programs
		map<GLuint, ShaderProgram*> programs;

		//Map of all registered shaders
		map<GLuint, Shader*> shaders;

	public:
		//Singleton access function
		static ShaderManager& get();

#pragma region Programs

		//Generate program with automatic replacement
		GLuint createProgram(const string& program_name, const string& vertex_shader_name = "", const string& fragment_shader_name = "", bool replace_existing = false);

		//Generate program with automatic replacement
		GLuint createProgram(const string& program_name, const GLuint& vertex_shader_id = 0, const GLuint& fragment_shader_id = 0, bool replace_existing = false);

		//Generate program with automatic replacement and fill a ptr container for it
		bool createProgram(const string& program_name, ShaderProgram*& container, const string& vertex_shader_name = "", const string& fragment_shader_name = "", bool replace_existing = false);

		//Generate program with automatic replacement and fill a ptr container for it
		bool createProgram(const string& program_name, ShaderProgram*& container, const GLuint& vertex_shader_id = 0, const GLuint& fragment_shader_id = 0, bool replace_existing = false);

		//Add program to programs list
		bool addProgram(string program_name, ShaderProgram* shader_program, bool replace_existing = false);

		//Remove program from list
		bool removeProgram(const GLuint& id);
		bool removeProgram(const string& name);

		//Get program ID from programs list
		GLuint getProgram(const string& name) const;

		//Get program ptr with given id
		bool getProgram(const GLuint& program_id, ShaderProgram*& program) const;

		//Get program ptr with given name
		GLuint getProgram(const string& program_name, ShaderProgram*& program) const;

		//Attach a shader to a program
		bool attachShader(const GLuint& program_id, const GLuint& shader_id, const bool& replace_existing);
		bool attachShader(const string& program_name, const GLuint& shader_id, const bool& replace_existing);
		bool attachShader(const string& program_name, const string& shader_name, const bool& replace_existing);

		//Detach shader from a program
		bool detachShader(const string& program_name, const string& shader_name);
		bool detachShader(const GLuint& program_id, const string& shader_name);

		//Detach shader from a program
		bool detachShader(const GLuint& program_id, const GLuint& shader_id);

		//Find an attribute in the attached shaders and add it to the attributes list
		bool addAttribute(const GLuint& program_id, const string& att_name);
		bool addAttribute(const string& program_name, const string& att_name);

		//Find an attribute in the attached shaders and add it to the attributes list
		bool setAttributeLocation(const GLuint& program_id, const string& att_name, const GLint& att_location);
		bool setAttributeLocation(const string& program_name, const string& att_name, const GLint& att_location);

		//Gets attribute from program and return it's location
		GLint getAttributeLocation(const GLuint& program_id, const string& att_name) const;
		GLint getAttributeLocation(const string& program_name, const string& att_name) const;

		//Gets attributes array from program and return it's size
		GLuint getAttributes(const GLuint& program_id, ShaderAttribute*& container) const;
		GLuint getAttributes(const string& program_name, ShaderAttribute*& container) const;

		//Remove an attribute from given program
		bool remAttribute(const string& program_name, const string& att_name);

		//Link shader program with given name or ID
		bool linkProgram(const string& program_name);
		bool linkProgram(const GLuint& program_id);

		//Use program with given name
		bool useProgram(const string& program_name);
		
		//Use program with given ID
		bool useProgram(const GLuint& program_id);

		//Return current program in use name
		string getCurrentProgram();

#pragma endregion

#pragma region Shaders

		//Generate shader with automatic replacement
		GLuint createShader(const string& name, const string& data, const GLuint& type, bool replace_existing = false);

		//Add shader to shaders list
		bool addShader(string shader_name, Shader* shader, bool replace_existing = false);

		//Get program ptr with given id
		bool getShader(const GLuint& shader_id, Shader*& shader) const;

		//Get shader ptr with given name
		GLuint getShader(const string& shader_name, Shader*& shader) const;

		//Get shader ID from shader list
		GLuint getShader(const string& name) const;

		//Remove shader from dictionary
		bool removeShader(const string& name, bool force_detach = false);

#pragma endregion

	};
}