#pragma once

//Define as using static GLEW
#define GLEW_STATIC

//Iostream for output
#include <iostream>

//Main Includes
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <GLM\glm.hpp>
#include <GLM\gtc\constants.hpp>

//Ravine Includes
#include "Common.h"
#include "Debug.h"
#include "gShaderManager.h"
#include "gVertexBuffer.h"
#include "gVertexArray.h"
#include "mFileLoader.h"

//Raytracer Includes
#include "Ray.h"
#include "gRayTracer.h"
#include "Primitives.h"
#include "PostProcessDecorator.h" 

//Typedefs
typedef const int cint;
typedef const char* str;

//Defines
#define rvApp rav::App::getApp()

//Ravine Namespace
namespace rav
{
	//The Main Application Class
	class App
	{
	private:
		//Private constructor and destructors for Singleton
		App();
		~App();

		//GLFW Window
		GLFWwindow* window;
		VertexArray* screenQuadVAO;
		VertexBuffer* screenQuadVBO;
		GLuint defaultPrg;
		PostProcess *postProcess;

		void CreateDefaultProgram();
		void CreateScreenQuad();
		void CreatePostProcess();
	public:
		//GLFW error callback
		static void error_callback(int error, const char* description)
		{
			fprintf(stderr, "Error: %s\n", description);
		}

		//GLFW reshape callback
		static void reshape(GLFWwindow* win, int w, int h)
		{
			//Update viewport on resize
			glViewport(0, 0, w, h);
		}

		//Delete Override operations
		App(App const&) = delete;
		void operator=(App const&) = delete;

		//Singleton Instantiator
		static App& getApp();

		//Initialization function
		int Initialize(cint &width = 1360, cint &height = 768, str name = "Default Window", bool fullscreen = true, bool vsync = false);

		//While function (Run to get into loop)
		int Run();

		//Clear all resources before existing
		int End();

	};
}