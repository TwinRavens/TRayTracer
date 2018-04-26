#include "App.h"

//STB Image Includes
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Using Ravine namespace
using namespace rav;

App::App()
{
}


App::~App()
{
	//Terminate GLFW
	glfwTerminate();
}

App & App::getApp()
{
	//Guaranteed to be destroyed
	static App app;

	return app;
}

void _update_fps_counter(GLFWwindow* window) {
	static double previous_seconds = glfwGetTime();
	static int frame_count;
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;
	if (elapsed_seconds > 0.12) {
		previous_seconds = current_seconds;
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		sprintf_s(tmp, "opengl @ fps: %.2f", fps);
		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}
	frame_count++;
}

int App::Initialize(cint &width, cint &height, str name, bool fullscreen)
{
	//Try to initialize GLFW
	if (!glfwInit())
	{
		//Debug error
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	//Link error callback function
	glfwSetErrorCallback(App::error_callback);

	//Monitor Holder variable
	GLFWmonitor *monitor = NULL;

	//Get primary monitor if fullscreen pretended
	if (fullscreen)
		monitor = glfwGetPrimaryMonitor();

	//Create application window
	window = glfwCreateWindow(width, height, name, monitor, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}

	//Now that our windows is has been set, set reshape callback
	glfwSetWindowSizeCallback(window, App::reshape);

	//Set as current active context
	glfwMakeContextCurrent(window);

	//Start GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	//Check OpenGL version
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	assert(version[0] > '3' && "Installed version of OpenGL doesn't support compute shader");

	//For proper rendering
	glEnable(GL_DEPTH_TEST);	//Enable depth-testing
	glDepthFunc(GL_LESS);		//Depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);		//Cull faces whose normals don't point towards the camera

	//Texture2D enabling
	glEnable(GL_TEXTURE_2D);	//Enable 2D Texture slot

	//Enable caching of buttons pressed
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

	//Set a nice blue background
	glClearColor(0.0f, 0.4509803921568627f, 0.8980392156862745f, 1.0f);

	//Initialize Raytracer
	RayTracer::Setup(width, height, 2);

	//Return no error message
	return 0;
}

int App::Run()
{
	char* vertex_shader;
	rvLoadFile("./data/vertex_uv.vs", vertex_shader, true);

	char* fragment_shader;
	rvLoadFile("./data/fragment_base.fs", fragment_shader, true);

	GLuint vs = rvCreateShader("vertex_uv_vs", vertex_shader, RV_VERTEX_SHADER);

	GLuint fs = rvCreateShader("fragment_base_vs", fragment_shader, RV_FRAGMENT_SHADER);

	//Create program
	GLuint pr = rvCreateProgram("test_pr", vs, fs);				//Create program with two shaders attached
	//rvSetAttributeLoc(pr, "vertex_position", 0);				//Set attribute location (before linking!)
	//rvSetAttributeLoc(pr, "vertex_coords", 1);				//Set attribute location (before linking!)
	rvLinkProgram(pr);											//Link program
	GLint vp_loc = rvGetAttributeLoc(pr, "vertex_position");	//Get attribute location (after linking!)
	GLint vc_loc = rvGetAttributeLoc(pr, "vertex_coords");		//Get attribute location (after linking!)

#pragma region ScreenQuad Generation

	float points[] = {
		//Position			  //UV
		-1.0f, -1.0f,  0.0f,	0.0f,  0.0f, //Bottom left
		1.0f,  1.0f,  0.0f,	1.0f,  1.0f, //Top-right
		-1.0f,  1.0f,  0.0f,	0.0f,  1.0f, //Top-left
		-1.0f, -1.0f,  0.0f,	0.0f,  0.0f, //Bottom-left
		1.0f, -1.0f,  0.0f,	1.0f,  0.0f, //Bottom-right
		1.0f,  1.0f,  0.0f,	1.0f,  1.0f  //Top-right
	};


	//Identity Matrix
	float matrix[] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	//Create VertexBuffer Object
	VertexBuffer vbo;

	//Add a description to it's buffer
	vbo.AddBufferDescriptor({		//Vertex Position Attribute
		vp_loc,						//Location ID
		3,							//Size of attribute (3 = XYZ)
		GL_FLOAT,					//Type of attribute
		false,						//Not normalized
		sizeof(float) * 5,			//Size of buffer block per vertex (3 for XYZ and 2 for UV)
		(void*)(0 * sizeof(float))	//Stride of 0 bytes (starts at the beginning of the block)
	});
	vbo.AddBufferDescriptor({		//Vertex UV Attribute
		vc_loc,						//Location ID
		2,							//Size of attribute (2 = UV)
		GL_FLOAT,					//Type of attribute
		false,						//Not normalized
		sizeof(float) * 5,			//Size of buffer block per vertex (3 for XYZ and 2 for UV)
		(void*)(3 * sizeof(float))	//Stride of 3 bytes (starts 3 bytes away from the beginning of the block)
	});

	//Copy data to VertexBuffer Object
	vbo.Fill(sizeof(points), points);

	//Create VertexArray Object
	VertexArray vao;

	//Enable locations for the created shader program
	vao.EnableLocations(pr);

	//Use VBO for setting data pointers
	vbo.SetAttributePointers();

#pragma endregion

	////Load texture with STB Image
	//int width, height, nrChannels;
	//unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//	stbi_image_free(data);
	//}
	//else
	//{
	//	rvDebug.Log("Failed to load texture", RV_ERROR_MESSAGE);
	//}

	while (!glfwWindowShouldClose(window))
	{
		_update_fps_counter(window);

		//===============COMPUT RAYTRACING HERE====================
		GLint raytracePreview = RayTracer::Compute();
		//===============COMPUT RAYTRACING HERE====================

#pragma region Draw Raytracer Output

		//Clear back color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use shader program
		rvUseProgram(pr);

		//Texture binding
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, raytracePreview);

		//Bind VAO
		vao.Bind();

		//Draw given VBO
		vbo.Draw();

#pragma endregion

		//Swap front and back buffers
		glfwSwapBuffers(window);

		//Poll for and process events
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, 1);
		}

	}

	//Return Sucessfully Exit
	return EXIT_SUCCESS;
}

int App::End()
{
	return 0;
}
