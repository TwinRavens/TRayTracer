#pragma once

//Main Includes
#include <GL\glew.h>
#include <GLM\glm.hpp>
#include <GLFW\glfw3.h>

//STD Includes
#include <string>

//Ravine Includes
#include "Debug.h"
#include "mFileLoader.h"

//Raytracer Includes
#include "Ray.h"
#include "Primitives.h"
#include "Material.h"
#include "Light.h"
#include "RayFileLoader.h"

//Defines
#define MAX_DEPTH 10

namespace rav
{
	class RayTracer
	{
	private:
		ObjectData * objData;
		int screenArea;
		Ray* rays;


		//Raytracer programs
		GLint setupProgram, collisionProgram, shadingProgram;

		//Screen data
		int width, height, raysSize, depthLevel;
		GLuint screenBuffer, diffuseBuffer, specularBuffer;

		//Shader Storage Buffer Objects
		GLuint raysBuffer, rayHitsBuffer;

		//Rays generation
		GLint generateRays();

		//Load Compute Shaders
		GLint loadPrograms();

		//Setup pass
		GLint setupPass();

		//Collision pass
		GLint collisionPass(int depth_level);

		//Shading pass
		GLint shadingPass(int depth_level);

		void GenerateScreenBuffer(GLuint& texture);

	public:
		RayTracer();

		//Set up screen buffers, load shaders
		GLint Setup(int screen_width, int screen_height, int depth_level = 2);

		//Renders a full image with given trace depth
		GLint Compute();

		GLuint getScreenBufferId();
		GLuint getDiffuseBufferId();
		GLuint getSpecularBufferId();
		glm::mat4 cameraRot;
		glm::vec4 cameraPos = glm::vec4(0, 0, 10, 1);

	};

}