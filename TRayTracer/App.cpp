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

	//Return no error message
	return 0;
}

int App::Run()
{
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

	char* vertex_shader;
	rvLoadFile("vertex_uv.vs", vertex_shader, true);

	char* fragment_shader;
	rvLoadFile("fragment_base.fs", fragment_shader, true);

	GLuint vs = rvCreateShader("vertex_uv_vs", vertex_shader, RV_VERTEX_SHADER);

	GLuint fs = rvCreateShader("fragment_base_vs", fragment_shader, RV_FRAGMENT_SHADER);

	//Create program
	GLuint pr = rvCreateProgram("test_pr", vs, fs);				//Create program with two shaders attached
	//rvSetAttributeLoc(pr, "vertex_position", 0);				//Set attribute location (before linking!)
	//rvSetAttributeLoc(pr, "vertex_coords", 1);				//Set attribute location (before linking!)
	rvLinkProgram(pr);											//Link program
	GLint vp_loc = rvGetAttributeLoc(pr, "vertex_position");	//Get attribute location (after linking!)
	GLint vc_loc = rvGetAttributeLoc(pr, "vertex_coords");		//Get attribute location (after linking!)

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

	//Generate texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Define texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);		//Repeat out of bounds UVs
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);		//Repeat out of bounds UVs
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set Image sampling filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//Set Image sampling filtering

	//Set Image sizes
	int width = 1920, height = 1080;

	//Bind Image Texture to layout 0
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

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

#pragma region Setup Compute Shader Variables
	int work_grp_cnt[3];

	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

	printf("max global (total) work group size x:%i y:%i z:%i\n",
		work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

	int work_grp_size[3];

	//
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

	printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
		work_grp_size[0], work_grp_size[1], work_grp_size[2]);

	//
	int work_grp_inv;

	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	printf("max local work group invocations %i\n", work_grp_inv);

#pragma endregion
#pragma region Load Compute Shader
	//Load Compute Shader
	char* compute_shader;
	rvLoadFile("compute_shader.glcs", compute_shader, true);

	GLuint ray_shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(ray_shader, 1, &compute_shader, NULL);
	glCompileShader(ray_shader);
	GLint isCompiled = 0;
	glGetShaderiv(ray_shader, GL_COMPILE_STATUS, &isCompiled);

	//If got an error
	if (isCompiled == GL_FALSE)
	{
		//Get size of info log
		GLint maxLength = 0;
		glGetShaderiv(ray_shader, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		GLchar *errorLog = new GLchar[maxLength];
		glGetShaderInfoLog(ray_shader, maxLength, &maxLength, &errorLog[0]);

		//Error header
		rvDebug.Log("Error compiling compute shader ", Debug::Error);

		//Create message
		string msg((const char*)errorLog);

		//Pop last \0 char
		msg.pop_back();

		//Log Error through Debugger
		rvDebug.Log(msg, Debug::Error);

		//Don't leak the shader
		glDeleteShader(ray_shader);
	}
	else
	{
		rvDebug.Log("Sucessfully compiled!\n");
	}

#pragma endregion

	GLuint ray_program = glCreateProgram();
	glAttachShader(ray_program, ray_shader);
	glLinkProgram(ray_program);

	//Lunch Compute Shader!
	glUseProgram(ray_program);
	glDispatchCompute((GLuint)width, (GLuint)height, 1);

	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	while (!glfwWindowShouldClose(window))
	{
		//Clear back color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use shader program
		rvUseProgram(pr);

		//Texture binding
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		//Bind VAO
		vao.Bind();

		//Draw given VBO
		vbo.Draw();

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
