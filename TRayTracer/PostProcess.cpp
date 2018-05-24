#include "PostProcess.h"

void PostProcess::SetupProgram(GLuint vertexShader, GLuint fragmentShader)
{
	string s = "post_process_v" + std::to_string(vertexShader) + "_f" + std::to_string(fragmentShader);
	program = rvGetProgram(s);
	if (program == 0) {
		program = rvCreateProgram(s, vertexShader, fragmentShader);
		rvLinkProgram(program);
	}

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

void PostProcess::CreateFBO()
{
	glGenFramebuffers(1, &FBO);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputBuffer, 0);

	//Check whether or not FrameBuffer is okay with GPU capabilities
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		rvDebug.Log("Framebuffer error Status: " + std::to_string(status), rav::Debug::Error);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#pragma region Constructors
PostProcess::PostProcess(GLuint vertexShader, GLuint fragmentShader, GLuint outputBuffer)
{
	SetupProgram(vertexShader, fragmentShader);
	this->outputBuffer = outputBuffer;
	CreateFBO();
}

PostProcess::PostProcess(GLuint vertexShader, GLuint fragmentShader, int width, int height)
{
	SetupProgram(vertexShader, fragmentShader);
	CreateOutputBuffer(width, height);
	CreateFBO();

}

PostProcess::PostProcess(GLuint programID, int width, int height)
{
	this->program = programID;
	CreateOutputBuffer(width, height);
	CreateFBO();

}

PostProcess::PostProcess(GLuint programID, GLuint outputBuffer)
{
	this->program = programID;
	this->outputBuffer = outputBuffer;
	CreateFBO();
}

#pragma endregion

PostProcess::~PostProcess()
{

}

GLuint PostProcess::Process(GLuint input)
{
	if (screenQuadVAO == nullptr) {

		rvDebug.Log("ScreenQuad of Post Process not set", rav::Debug::Error);
		return -1;
	}

	//Change input texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, input);
	
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rvUseProgram(this->program);

	screenQuadVAO->Bind();
	screenQuadVBO->Draw();

	//unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	return this->outputBuffer;
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

void PostProcess::setScreenQuad(rav::VertexArray * screenQuadVAO, rav::VertexBuffer * screenQuadVBO)
{
	this->screenQuadVAO = screenQuadVAO;
	this->screenQuadVBO = screenQuadVBO;
}
void PostProcess::getScreenQuad(rav::VertexArray ** screenQuadVAO, rav::VertexBuffer ** screenQuadVBO)
{
	*screenQuadVAO = this->screenQuadVAO;
	*screenQuadVBO = this->screenQuadVBO;
}



