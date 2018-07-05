#pragma once
#include "PostProcessDecorator.h"
class PostProcessDecoratorFloatParam : public PostProcessDecorator
{
protected:
	PostProcess * postProcess;
	GLfloat* parameter;
	string paramName;
public:
	PostProcessDecoratorFloatParam(PostProcess *process, GLfloat& param, const string& paramName, GLuint vertexShader, GLuint fragmentShader, GLuint outputBuffer);
	PostProcessDecoratorFloatParam(PostProcess *process, GLfloat& param, const string& paramName, GLuint vertexShader, GLuint fragmentShader, int width, int height);
	PostProcessDecoratorFloatParam(PostProcess *process, GLfloat& param, const string& paramName, GLuint programID, int width, int height);
	PostProcessDecoratorFloatParam(PostProcess *process, GLfloat& param, const string& paramName, GLuint programID, GLuint outputBuffer);
	~PostProcessDecoratorFloatParam();
	void PreProcess();
};

