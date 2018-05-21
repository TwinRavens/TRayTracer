#include "PostProcess.h"




void PostProcess::SetupProgram(GLuint vertexShader, GLuint fragmentShader)
{
	program = rvCreateProgram("post_process_v" + std::to_string(vertexShader) + "_f" + std::to_string(fragmentShader),
		vertexShader, fragmentShader);
}

void PostProcess::CreateOutputBuffer(int width, int height)
{
	glGenTextures(1, &outputBuffer);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, outputBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	//Define texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//Repeat out of bounds UVs
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//Repeat out of bounds UVs
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//Set Image sampling filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//Set Image sampling filtering
}

#pragma region Constructors
PostProcess::PostProcess(GLuint vertexShader, GLuint fragmentShader, GLuint outputBuffer)
{
	SetupProgram(vertexShader, fragmentShader);
	this->outputBuffer = outputBuffer;
}

PostProcess::PostProcess(GLuint vertexShader, GLuint fragmentShader, int width, int height)
{
	SetupProgram(vertexShader, fragmentShader);
	CreateOutputBuffer(width, height);
}

PostProcess::PostProcess(GLuint programID, int width, int height)
{
	this->program = programID;
	CreateOutputBuffer(width, height);
}

PostProcess::PostProcess(GLuint programID, GLuint outputBuffer)
{
	this->program = programID;
	this->outputBuffer = outputBuffer;
}

#pragma endregion

PostProcess::~PostProcess()
{
}


GLuint PostProcess::Process(GLuint input)
{
	if (screenQuadVAO == nullptr) {

		return -1;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rvUseProgram(this->program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, input);

	//Bind VAO
	screenQuadVAO->Bind();

	//Draw given VBO
	screenQuadVBO->Draw();

	return this->outputBuffer;
}

void PostProcess::setScreenQuad(rav::VertexArray * screenQuadVAO, rav::VertexBuffer * screenQuadVBO)
{
	this->screenQuadVAO = screenQuadVAO;
	this->screenQuadVBO = screenQuadVBO;
}

void PostProcess::CreateScreenQuad()
{
	screenQuadVBO = new rav::VertexBuffer();
	screenQuadVAO = new rav::VertexArray();

	GLint vp_loc = rvGetAttributeLoc(program, "vertex_position");	//Get attribute location (after linking!)
	GLint vc_loc = rvGetAttributeLoc(program, "vertex_coords");		//Get attribute location (after linking!)

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


	//Add a description to it's buffer
	screenQuadVBO->AddBufferDescriptor({		//Vertex Position Attribute
		vp_loc,						//Location ID
		3,							//Size of attribute (3 = XYZ)
		GL_FLOAT,					//Type of attribute
		false,						//Not normalized
		sizeof(float) * 5,			//Size of buffer block per vertex (3 for XYZ and 2 for UV)
		(void*)(0 * sizeof(float))	//Stride of 0 bytes (starts at the beginning of the block)
		});
	screenQuadVBO->AddBufferDescriptor({		//Vertex UV Attribute
		vc_loc,						//Location ID
		2,							//Size of attribute (2 = UV)
		GL_FLOAT,					//Type of attribute
		false,						//Not normalized
		sizeof(float) * 5,			//Size of buffer block per vertex (3 for XYZ and 2 for UV)
		(void*)(3 * sizeof(float))	//Stride of 3 bytes (starts 3 bytes away from the beginning of the block)
		});

	//Copy data to VertexBuffer Object
	screenQuadVBO->Fill(sizeof(points), points);


	//Enable locations for the created shader program
	screenQuadVAO->EnableLocations(program);

	//Use VBO for setting data pointers
	screenQuadVBO->SetAttributePointers();

}



