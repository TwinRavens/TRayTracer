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

//Defines
#define MAX_DEPTH 10

namespace rav
{
	static class RayTracer
	{
	private:
		//Raytracer programs
		static GLint collisionProgram, shadingProgram;

		//Screen data
		static int width, height, raysSize, depthLevel;
		static GLuint screenBuffer;

		//Shader Storage Buffer Objects
		static GLuint raysBuffer, rayHitsBuffer;

		//Rays generation
		static GLint generateRays();

		//Load Compute Shaders
		static GLint loadPrograms();

		//Collision pass
		static GLint collisionPass(int depth_level);

		//Shading pass
		static GLint shadingPass(int depth_level);

		//Private constructor to avoid overriding
		RayTracer();

	public:
		//Set up screen buffers, load shaders
		static GLint Setup(int screen_width, int screen_height, int depth_level = 4);

		//Renders a full image with given trace depth
		static GLint Compute();

	};

}