#include "App.h"

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

	//Set a nice blue background
	glClearColor(0.0f, 0.4509803921568627f, 0.8980392156862745f, 1.0f);

	//Return no error message
	return 0;
}

int App::Run()
{
	float points[] = {
		-0.5f, -0.5f,  0.0f, //Vertex
		 1.0f,  0.0f,  0.0f, //Color
		 0.5f, -0.5f,  0.0f, //Vertex
		 0.0f,  1.0f,  0.0f, //Color
		 0.0f,  0.5f,  0.0f, //Vertex
		 0.0f,  0.0f,  1.0f	 //Color
	};

	float matrix[] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	char* vertex_shader;
	rvLoadFile("vertex_color.vs", vertex_shader, true);

	char* fragment_shader;
	rvLoadFile("base_frag.fs", fragment_shader, true);

	GLuint vs = rvCreateShader("vertex_color_vs", vertex_shader, RV_VERTEX_SHADER);

	GLuint fs = rvCreateShader("base_fragment_fs", fragment_shader, RV_FRAGMENT_SHADER);

	//Create program
	GLuint pr = rvCreateProgram("test_pr", vs, fs);				//Create program with two shaders attached
	rvSetAttributeLoc(pr, "vertex_position", 0);				//Set attribute location (before binding!)
	rvSetAttributeLoc(pr, "vertex_color", 1);					//Set attribute location (before binding!)
	rvLinkProgram(pr);											//Link program
	GLint vp_loc = rvGetAttributeLoc(pr, "vertex_position");	//Get attribute location (after binding!)
	GLint vc_loc = rvGetAttributeLoc(pr, "vertex_color");		//Get attribute location (after binding!)

	//Create VertexBuffer Object
	VertexBuffer vbo;

	//Add a description to it's buffer
	vbo.AddBufferDescriptor({		//Vertex Position Attribute
		vp_loc,						//Location ID
		3,							//Size of attribute (3 = XYZ)
		GL_FLOAT,					//Type of attribute
		false,						//Not normalized
		sizeof(float) * 6,			//Size of buffer block per vertex (3 for XYZ and 3 for RGB)
		(void*)(0 * sizeof(float))	//Stride of 0 bytes (starts at the beginning of the block)
	});
	vbo.AddBufferDescriptor({		//Vertex Color Attribute
		vc_loc,						//Location ID
		3,							//Size of attribute (3 = RGB)
		GL_FLOAT,					//Type of attribute
		false,						//Not normalized
		sizeof(float) * 6,			//Size of buffer block per vertex (3 for XYZ and 3 for RGB)
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

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use shader program
		rvUseProgram(pr);

		//Bind VAO
		vao.Bind();

		//Draw given VBO
		vbo.Draw();

		//Swap front and back buffers
		glfwSwapBuffers(window);

		//Poll for and process events
		glfwPollEvents();
	}

	//Return Sucessfully Exit
	return EXIT_SUCCESS;
}

int App::End()
{
	return 0;
}
