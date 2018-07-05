#include "PostProcessDecorator.h"



void PostProcessDecorator::Setup(PostProcess * process)
{
	this->postProcess = process;
	this->postProcess->getScreenQuad(&screenQuadVAO, &screenQuadVBO);
}

PostProcessDecorator::PostProcessDecorator(PostProcess *process, GLuint vertexShader, GLuint fragmentShader, GLuint outputBuffer) : PostProcess(vertexShader, fragmentShader, outputBuffer)
{
	Setup(process);
}

PostProcessDecorator::PostProcessDecorator(PostProcess *process, GLuint vertexShader, GLuint fragmentShader, int width, int height) : PostProcess(vertexShader, fragmentShader, width, height)
{
	Setup(process);
}

PostProcessDecorator::PostProcessDecorator(PostProcess *process, GLuint programID, int width, int height) : PostProcess(programID, width, height)
{
	Setup(process);
}

PostProcessDecorator::PostProcessDecorator(PostProcess *process, GLuint programID, GLuint outputBuffer) : PostProcess(programID, outputBuffer)
{
	Setup(process);
}

PostProcessDecorator::~PostProcessDecorator()
{
	if(this->postProcess != nullptr)
	delete this->postProcess;
}

void PostProcessDecorator::PreProcess()
{
}

GLuint PostProcessDecorator::Process(GLuint input)
{
	GLuint i = postProcess->Process(input);
	PreProcess();
	return PostProcess::Process(i);
}



