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
		static int width, height;
		static GLuint frontBuffer, backBuffer;

		//Shader Storage Buffer Objects
		static GLuint raysBuffer;

		//Rays generation
		static GLint generateRays();

		//Load Compute Shaders
		static GLint loadPrograms();

		//Collision pass
		static GLint computeCollision();

		//Shading pass
		static GLint computeShading();

		//Private constructor to avoid overriding
		RayTracer();

	public:
		//Set up screen buffers, load shaders
		static GLint Setup(int screen_width, int screen_height);

		//Renders a full image with given trace depth
		static GLint Compute(int depth_level = 4);

	};

}