#include "PostProcessDecoratorFloatParam.h"

PostProcessDecoratorFloatParam::PostProcessDecoratorFloatParam(PostProcess * process, GLfloat & param, const string & paramName, GLuint vertexShader, GLuint fragmentShader, GLuint outputBuffer) :
	PostProcessDecorator(process, vertexShader, fragmentShader, outputBuffer)
{
	this->parameter = &param;
	this->paramName = paramName;
}

PostProcessDecoratorFloatParam::PostProcessDecoratorFloatParam(PostProcess * process, GLfloat & param, const string & paramName, GLuint vertexShader, GLuint fragmentShader, int width, int height) :
	PostProcessDecorator(process, vertexShader, fragmentShader, width, height)
{
	this->parameter = &param;
	this->paramName = paramName;
}

PostProcessDecoratorFloatParam::PostProcessDecoratorFloatParam(PostProcess * process, GLfloat & param, const string & paramName, GLuint programID, int width, int height) :
	PostProcessDecorator(process, programID, width, height)
{
	this->parameter = &param;
	this->paramName = paramName;
}

PostProcessDecoratorFloatParam::PostProcessDecoratorFloatParam(PostProcess * process, GLfloat & param, const string & paramName, GLuint programID, GLuint outputBuffer) :
	PostProcessDecorator(process, programID, outputBuffer)
{
	this->parameter = &param;
	this->paramName = paramName;
}

PostProcessDecoratorFloatParam::~PostProcessDecoratorFloatParam()
{
}

void PostProcessDecoratorFloatParam::PreProcess()
{
	GLint parameterLoc = glGetUniformLocation(program, paramName.c_str());
	if (parameter != nullptr)
		glProgramUniform1f(program, parameterLoc, *parameter);
}
