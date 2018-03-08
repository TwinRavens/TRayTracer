#include "gShaderManager.h"

using namespace rav;

rav::ShaderManager::ShaderManager() : currentProgram(nullptr)
{

}

rav::ShaderManager::~ShaderManager()
{
	//Go through every shader on the map and delete it
	map<GLuint, Shader*>::iterator shader_it;
	for (shader_it = shaders.begin(); shader_it != shaders.end(); shader_it++)
	{
		delete(shader_it->second);
	}

	//Go through every program on the map and delete it
	map<GLuint, ShaderProgram*>::iterator program_it = programs.begin();
	for (program_it = programs.begin(); program_it != programs.end(); program_it++)
	{
		delete(program_it->second);
	}
}

ShaderManager & rav::ShaderManager::get()
{
	//Guaranteed to be destroyed
	static ShaderManager shaderManager;

	return shaderManager;
}

GLuint rav::ShaderManager::createProgram(const string& program_name, const string& vertex_shader_name, const string& fragment_shader_name, bool replace_existing)
{
	//Check if program exists
	ShaderProgram *shp;
	if (getProgram(program_name, shp))
	{
		if (replace_existing)
		{
			//Log info
			rvDebug.Log("Program " + program_name + " already exists, replace enabled, overriding...");

			//Remove from map
			programs.erase(shp->getID());

			//Remove last shader
			delete shp;
		}
		else
		{
			//Log info
			rvDebug.Log("Program " + program_name + " already exists, replace disabled!\n", RV_ERROR_MESSAGE);

			//Return error code
			return 0;
		}
	}

	//Allocate memory
	shp = new ShaderProgram(program_name);

	//If vertex shader name given
	if (!vertex_shader_name.empty())
	{
		//Look for it in shader's list
		GLuint vt_id = getShader(vertex_shader_name);

		//If ID is valid, attach that shader
		if (vt_id != 0)
			shp->Attach(vt_id, RV_VERTEX_SHADER);
		else //Return error code
			return 0;
	}

	//If fragment shader name given
	if (!fragment_shader_name.empty())
	{
		//Look for it in shader's list
		GLuint fr_id = getShader(fragment_shader_name);

		//If ID is valid, attach that shader
		if (fr_id != 0)
			shp->Attach(fr_id, RV_FRAGMENT_SHADER);
		else //Return error code
			return 0;
	}

	//Add program to list
	if (!addProgram(program_name, shp, replace_existing))
	{
		//Return error code
		return 0;
	}

	//Return shader program ID
	return shp->getID();
}

GLuint rav::ShaderManager::createProgram(const string & program_name, const GLuint & vertex_shader_id, const GLuint & fragment_shader_id, bool replace_existing)
{
	//Check if program exists
	ShaderProgram *shp;
	if (getProgram(program_name, shp))
	{
		if (replace_existing)
		{
			//Log info
			rvDebug.Log("Program " + program_name + " already exists, replace enabled, overriding...");

			//Remove from map
			programs.erase(shp->getID());

			//Remove last shader
			delete shp;
		}
		else
		{
			//Log info
			rvDebug.Log("Program " + program_name + " already exists, replace disabled!\n", RV_ERROR_MESSAGE);

			//Return error code
			return 0;
		}
	}

	//Allocate memory
	shp = new ShaderProgram(program_name);

	//If ID is valid, attach that shader
	if (vertex_shader_id != 0)
		shp->Attach(vertex_shader_id, RV_VERTEX_SHADER);
	else //Return error code
		return 0;

	//If ID is valid, attach that shader
	if (fragment_shader_id != 0)
		shp->Attach(fragment_shader_id, RV_FRAGMENT_SHADER);
	else //Return error code
		return 0;

	//Add program to list
	if (!addProgram(program_name, shp, replace_existing))
		return 0; //Return error code

	//Return shader program ID
	return shp->getID();
}

bool rav::ShaderManager::createProgram(const string & program_name, ShaderProgram *& container, const string & vertex_shader_name, const string & fragment_shader_name, bool replace_existing)
{
	//Check if program exists
	ShaderProgram *shp;
	if (getProgram(program_name, shp))
	{
		if (replace_existing)
		{
			//Log info
			rvDebug.Log("Program " + program_name + " already exists, replace enabled, overriding...");

			//Remove from map
			programs.erase(shp->getID());

			//Remove last shader
			delete shp;
		}
		else
		{
			//Log info
			rvDebug.Log("Program " + program_name + " already exists, replace disabled!\n", RV_ERROR_MESSAGE);

			//Return error code
			return false;
		}
	}

	//Allocate memory
	shp = new ShaderProgram(program_name);

	//If vertex shader name given
	if (!vertex_shader_name.empty())
	{
		//Look for it in shader's list
		GLuint vt_id = getShader(vertex_shader_name);

		//If ID is valid, attach that shader
		if (vt_id != 0)
			shp->Attach(vt_id, RV_VERTEX_SHADER);
		else //Return error code
			return false;
	}

	//If fragment shader name given
	if (!fragment_shader_name.empty())
	{
		//Look for it in shader's list
		GLuint fr_id = getShader(fragment_shader_name);

		//If ID is valid, attach that shader
		if (fr_id != 0)
			shp->Attach(fr_id, RV_FRAGMENT_SHADER);
		else //Return error code
			return false;
	}

	//Add program to list
	if (!addProgram(program_name, shp, replace_existing))
	{
		//Return error code
		return false;
	}

	//Copy program address to container
	container = shp;

	//Return shader success
	return true;
}

bool rav::ShaderManager::createProgram(const string & program_name, ShaderProgram *& container, const GLuint & vertex_shader_id, const GLuint & fragment_shader_id, bool replace_existing)
{
	//Check if program exists
	ShaderProgram *shp;
	if (getProgram(program_name, shp))
	{
		if (replace_existing)
		{
			//Log info
			rvDebug.Log("Program " + program_name + " already exists, replace enabled, overriding...");

			//Remove from map
			programs.erase(shp->getID());

			//Remove last shader
			delete shp;
		}
		else
		{
			//Log info
			rvDebug.Log("Program " + program_name + " already exists, replace disabled!\n", RV_ERROR_MESSAGE);

			//Return error code
			return false;
		}
	}

	//Allocate memory
	shp = new ShaderProgram(program_name);

	//If ID is valid, attach that shader
	if (vertex_shader_id != 0)
		shp->Attach(vertex_shader_id, RV_VERTEX_SHADER);
	else //Return error code
		return false;

	//If ID is valid, attach that shader
	if (fragment_shader_id != 0)
		shp->Attach(fragment_shader_id, RV_FRAGMENT_SHADER);
	else //Return error code
		return false;

	//Add program to list
	if (!addProgram(program_name, shp, replace_existing))
		return false; //Return error code

	//Copy program address to container
	container = shp;

	//Return shader program ID
	return true;
}

bool rav::ShaderManager::addProgram(string program_name, ShaderProgram* shader_program, bool replace_existing)
{
	//If shader not null
	if (shader_program)
	{
		//Check if another shader with such name is already registered
		map<GLuint, ShaderProgram*>::iterator it;
		for (it = programs.begin(); it != programs.end(); it++)
		{
			if (it->second->getName() == program_name)
			{
				//Shoudl replace?
				if (!replace_existing)
				{
					//Log Error through Debugger
					rvDebug.Log("Program of name: " + program_name + " already exists! Not overriding!\n", RV_ERROR_MESSAGE);

					//Shader Already Exists!
					return false;
				}

				//Log Warning through Debugger
				rvDebug.Log("Program of name: " + program_name + " already exists! Forced replacing enabled, overriding!\n"
					"Be sure NOT TO USE THAT PROGRAM afterwards!\n", RV_WARNING_MESSAGE);

				//Delete old shader
				delete it->second;

				//Erase from map
				programs.erase(it);
				break;
			}
		}

		//Add new shader
		programs.emplace(shader_program->getID(), shader_program);

		//Return Sucess
		return true;
	}

	//Log Error through Debugger
	rvDebug.Log("Shader " + program_name + " is NULL!\n", Debug::Error);

	//Return Error
	return false;
}

bool rav::ShaderManager::getProgram(const GLuint& program_id, ShaderProgram*& program) const
{
	//Go through every program in map
	map<GLuint, ShaderProgram*>::const_iterator it = programs.find(program_id);
	if (it != programs.cend())
	{
		//Assign pointer
		program = it->second;

		//Return success code
		return true;
	}

	//Log error
	rvDebug.Log("Could not find program with such ID!\n", RV_ERROR_MESSAGE);

	//Return error code
	return false;
}

GLuint rav::ShaderManager::getProgram(const string& program_name, ShaderProgram*& program) const
{
	//Get Program Ptr
	map<GLuint, ShaderProgram*>::const_iterator it;
	for (it = programs.cbegin(); it != programs.cend(); it++)
	{
		if (it->second->getName() == program_name)
		{
			//Update Shader Program ptr
			program = it->second;

			//Return success
			return it->first;
		}
	}

	//Log Error
	rvDebug.Log("Program " + program_name + " could not be found!\n", RV_WARNING_MESSAGE);

	//Return error code
	return 0;
}

bool rav::ShaderManager::attachShader(const GLuint & program_id, const GLuint & shader_id, const bool & replace_existing)
{
	//Get Shader and Program Ptr
	Shader *sh;
	ShaderProgram *pr;
	if (!getShader(program_id, sh) || !getProgram(program_id, pr))
		return false; //Return error code

	//Attach shader
	if (!pr->Attach(sh->getID(), sh->getType(), replace_existing))
		return false; //Return error code

	//Return Success
	return true;
}

GLuint rav::ShaderManager::getProgram(const string& program_name) const
{
	//Get shader
	map<GLuint, ShaderProgram*>::const_iterator it;
	for (it = programs.cbegin(); it != programs.cend(); it++)
	{
		if (it->second->getName() == program_name)
		{
			//Return program ID (SUCESS)
			return it->first;
		}
	}

	//Log error
	rvDebug.Log("Program " + program_name + " could not be found!\n", RV_ERROR_MESSAGE);

	//Return error code
	return 0;
}

bool rav::ShaderManager::removeProgram(const string& program_name)
{
	rvDebug.Log("Removing program " + program_name + "...");

	//Check if program exists on map
	ShaderProgram* pr;
	if (getProgram(program_name, pr))
	{
		//Delete from shaders map
		programs.erase(pr->getID());

		//Delete program from local and OpenGl's memory
		delete pr;

		//Return sucess code
		return true;
	}

	//Log warning (could not find shader)
	rvDebug.Log("Could not find program with name " + program_name + "!\n", Debug::Warning);

	//Return error code
	return false;
}

bool rav::ShaderManager::removeProgram(const GLuint &id)
{
	rvDebug.Log("Removing program with ID " + std::to_string(id) + "...");

	//Check if program exists on map
	ShaderProgram* pr;
	if (getProgram(id, pr))
	{
		//Delete from shaders map
		programs.erase(id);

		//Delete program from local and OpenGl's memory
		delete pr;

		//Return sucess code
		return true;
	}

	//Log warning (could not find shader)
	rvDebug.Log("Could not find program with ID " + std::to_string(id) + "!\n", Debug::Warning);

	//Return error code
	return false;
}

bool rav::ShaderManager::attachShader(const string& program_name, const string& shader_name, const bool& replace_existing)
{
	//Get Shader and Program Ptr
	Shader *sh;
	ShaderProgram *pr;
	if (!getShader(shader_name, sh) || !getProgram(program_name, pr))
		return false; //Return error code

	//Attach shader
	return pr->Attach(sh->getID(), sh->getType(), replace_existing);
}

bool rav::ShaderManager::attachShader(const string& program_name, const GLuint & shader_id, const bool & replace_existing)
{
	//Get Shader and Program Ptr
	Shader *sh;
	ShaderProgram *pr;
	if (!getShader(shader_id, sh) || !getProgram(program_name, pr))
		return false; //Return error code

	//Attach shader
	return pr->Attach(sh->getID(), sh->getType(), replace_existing);
}

bool rav::ShaderManager::detachShader(const string& program_name, const string& shader_name)
{
	//Get Shader and Program Ptr
	Shader *sh;
	ShaderProgram *pr;
	if (!getShader(shader_name, sh) || !getProgram(program_name, pr))
		return false; //Return error code

	//Detach shader
	return pr->Detach(sh->getID(), sh->getType());
}

bool rav::ShaderManager::detachShader(const GLuint& program_id, const string& shader_name)
{
	//Get Shader and Program Ptr
	Shader *sh;
	ShaderProgram *pr;
	if (!getShader(shader_name, sh) || !getProgram(program_id, pr))
		return false; //Return error code

	//Detach shader
	return pr->Detach(sh->getID(), sh->getType());
}

bool rav::ShaderManager::detachShader(const GLuint& program_id, const GLuint& shader_id)
{
	//Get Shader and Program Ptr
	Shader *sh;
	ShaderProgram *pr;
	if (!getShader(shader_id, sh) || !getProgram(program_id, pr))
		return false; //Return error code

	//Detach shader
	return pr->Detach(sh->getID(), sh->getType());
}

bool rav::ShaderManager::addAttribute(const string& program_name, const string& att_name)
{
	//Get shader program ptr
	ShaderProgram* shp;
	if (!getProgram(program_name, shp))
		return false; //Return error code

	//Add attribute
	return shp->addAttributeLocation(att_name);
}

bool rav::ShaderManager::addAttribute(const GLuint& program_id, const string& att_name)
{
	//Get shader program ptr
	ShaderProgram* shp;
	if (!getProgram(program_id, shp))
		return false; //Return error code

	//Add attribute
	return shp->addAttributeLocation(att_name);
}

bool rav::ShaderManager::setAttributeLocation(const string& program_name, const string& att_name, const GLint& att_location)
{
	//Get shader program ptr
	ShaderProgram* shp;
	if (!getProgram(program_name, shp))
		return false; //Return error code

	//Set attribute
	return shp->setAttributeLocation(att_name, att_location);
}

bool rav::ShaderManager::setAttributeLocation(const GLuint& program_id, const string& att_name, const GLint& att_location)
{
	//Get shader program ptr
	ShaderProgram* shp;
	if (!getProgram(program_id, shp))
		return false; //Return error code

	//Set attribute
	return shp->setAttributeLocation(att_name, att_location);
}

GLint rav::ShaderManager::getAttributeLocation(const string& program_name, const string& att_name) const
{
	//Get shader program ptr
	ShaderProgram* shp;
	if (!getProgram(program_name, shp))
		return -1; //Return error code (-1)

	//Return attribute location (or error code)
	return shp->getAttribute(att_name);
}

GLuint rav::ShaderManager::getAttributes(const GLuint & program_id, ShaderAttribute *& container) const
{
	//Get shader program ptr
	ShaderProgram* shp;
	if (!getProgram(program_id, shp))
		return 0; //Return error code (or zero size array)

	//Return attributes array size (or error code)
	return shp->getAttributes(container);
}

GLuint rav::ShaderManager::getAttributes(const string & program_name, ShaderAttribute *& container) const
{
	//Get shader program ptr
	ShaderProgram* shp;
	if (!getProgram(program_name, shp))
		return 0; //Return error code (or zero size array)

	//Return attributes array size (or error code)
	return shp->getAttributes(container);
}

GLint rav::ShaderManager::getAttributeLocation(const GLuint& program_id, const string & att_name) const
{
	//Get shader program ptr
	ShaderProgram* shp;
	if (!getProgram(program_id, shp))
		return -1; //Return error code (-1)

	//Return attribute location (or error code)
	return shp->getAttribute(att_name);
}

bool rav::ShaderManager::remAttribute(const string& program_name, const string& att_name)
{
	//Get shader program ptr
	ShaderProgram* shp;
	if (!getProgram(program_name, shp))
		return false; //Return error code

	//Remove attribute
	return shp->remAttribute(att_name);
}

bool rav::ShaderManager::linkProgram(const string& program_name)
{
	//Get shader program ptr
	ShaderProgram* shp;
	if (!getProgram(program_name, shp))
		return false; //Return error code

	//Link program
	return shp->Link();
}

bool rav::ShaderManager::linkProgram(const GLuint& program_id)
{
	//Get shader program ptr
	ShaderProgram* shp;
	if (!getProgram(program_id, shp))
		return false; //Return error code

	//Link program
	return shp->Link();
}

bool rav::ShaderManager::useProgram(const string& program_name)
{
	//Get shader program ptr
	ShaderProgram* shp;
	if (!getProgram(program_name, shp))
		return false; //Return error code

	//Use this program
	shp->Use();

	//Update current program
	currentProgram = shp;

	//Return Success Code
	return true;
}

bool rav::ShaderManager::useProgram(const GLuint& program_id)
{
	//Get shader program ptr
	ShaderProgram* shp;
	if (!getProgram(program_id, shp))
		return false; //Return error code

	//Use this program
	shp->Use();

	//Update current program
	currentProgram = shp;

	//Return Success Code
	return true;
}

string rav::ShaderManager::getCurrentProgram()
{
	//If has current program
	if (currentProgram)
	{
		//Log info
		rvDebug.Log("Current program being used is " + currentProgram->getName());

		//Return current program name
		return currentProgram->getName();
	}

	//Log warning
	rvDebug.Log("No program being use at the moment!\n", RV_WARNING_MESSAGE);

	//Return empty string as error
	return "";
}

GLuint rav::ShaderManager::createShader(const string& name, const string& data, const GLuint& type, bool replace_existing)
{
	//Create shader with given information
	Shader *sh = new Shader(data, type, name);

	//If shader compiled and was added sucessfully, return shader ID
	if (sh->Compile() && addShader(name, sh, replace_existing))
		return sh->getID();

	//Error when compiling or adding shader, clean-up
	delete sh;

	//Return error code
	return 0;
}

bool rav::ShaderManager::addShader(string shader_name, Shader* shader, bool replace_existing)
{
	//If shader not null
	if (shader)
	{
		Shader* sh;
		if (getShader(shader_name, sh))
		{
			if (!replace_existing)
			{
				//Log Error through Debugger
				rvDebug.Log("Shader of name: " + shader_name + " already exists! Not overriding!\n", RV_ERROR_MESSAGE);

				//Shader Already Exists!
				return false;
			}

			//Log Warning through Debugger
			rvDebug.Log("Shader of name: " + shader_name + " already exists! Forced replacing enabled, overriding!\n"
				"Be sure NOT TO USE THAT SHADER afterwards!\n", RV_WARNING_MESSAGE);

			//Erase from map
			shaders.erase(sh->getID());

			//Delete old shader
			delete sh;
		}

		//Add new shader
		shaders.emplace(shader->getID(), shader);

		//Return Sucess
		return true;
	}

	//Log Error through Debugger
	rvDebug.Log("Shader " + shader_name + " is NULL!\n", Debug::Error);

	//Return Error
	return false;
}

bool rav::ShaderManager::getShader(const GLuint& shader_id, Shader*& shader) const
{
	//Log info
	rvDebug.Log("Looking for shader with ID " + std::to_string(shader_id) + "...");

	//Go through every shader in map
	map<GLuint, Shader*>::const_iterator it = shaders.find(shader_id);
	if (it != shaders.cend())
	{
		//Assign pointer
		shader = it->second;

		//Log info
		rvDebug.Log("Shader " + shader->getName() + " found with such ID!");

		//Return success code
		return true;
	}

	//Log error
	rvDebug.Log("Could not find shader with such ID!\n", RV_WARNING_MESSAGE);

	//Return error code
	return false;
}

GLuint rav::ShaderManager::getShader(const string& shader_name, Shader*& shader) const
{
	//Get Shader Ptr
	map<GLuint, Shader*>::const_iterator it;
	for (it = shaders.cbegin(); it != shaders.cend(); it++)
	{
		if (it->second->getName() == shader_name)
		{
			//Update Shader ptr
			shader = it->second;

			//Return Shader ID
			return it->first;
		}
	}

	//Log Error
	rvDebug.Log("Shader " + shader_name + " could not be found!\n", RV_WARNING_MESSAGE);

	//Return error code
	return 0;
}

GLuint rav::ShaderManager::getShader(const string& shader_name) const
{
	//Get shader
	map<GLuint, Shader*>::const_iterator it;
	for (it = shaders.cbegin(); it != shaders.cend(); it++)
	{
		//If found it
		if (it->second->getName() == shader_name)
		{
			//Return Shader ID
			return it->first;
		}
	}

	//Log error
	rvDebug.Log("Shader " + shader_name + " could not be found!\n", RV_WARNING_MESSAGE);

	//Return error code
	return 0;
}

bool rav::ShaderManager::removeShader(const string& shader_name, bool force_detach)
{
	rvDebug.Log("Removing shader " + shader_name + "...");

	//Check if shader exists on map
	Shader* sh;
	if (getShader(shader_name, sh))
	{
		//If should detach shader
		if (force_detach)
		{
			//Shader Program iterator
			map<GLuint, ShaderProgram*>::iterator pr_it;

			//Go through every program and check which one uses that shader
			for (pr_it = programs.begin(); pr_it != programs.end(); pr_it++)
			{
				//If it does, detach it and log warning
				if (pr_it->second->isAttached(sh->getID(), sh->getType()))
				{
					//Log behaviour warning
					rvDebug.Log("Program " + pr_it->second->getName() + " uses that shader!" +
						" Be sure not to use such program before attaching another one!\n", RV_WARNING_MESSAGE);

					//Detach error
					pr_it->second->Detach(sh->getID(), sh->getType());
				}
			}
		}
		else
		{
			//Log info relative to deleting information
			rvDebug.Log("Shader won't be deleted from OpenGL until detached from all programs!");
		}

		//Delete from shaders map
		shaders.erase(sh->getID());

		//If it does, delete it from local and OpenGl's memory
		delete sh;

		//Return sucess code
		return true;
	}

	//Log warning (could not find shader)
	rvDebug.Log("Could not find shader with name " + shader_name + "!\n", Debug::Warning);

	//Return error code
	return false;
}