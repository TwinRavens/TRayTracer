#pragma once
#include "PostProcessDecorator.h"
class PostProcessDecoratorTexturized : public PostProcessDecorator
{
protected:
	PostProcess * postProcess;
	GLuint texture;
public:
	PostProcessDecoratorTexturized(PostProcess *process, GLuint texture, GLuint vertexShader, GLuint fragmentShader, GLuint outputBuffer);
	PostProcessDecoratorTexturized(PostProcess *process, GLuint texture, GLuint vertexShader, GLuint fragmentShader, int width, int height);
	PostProcessDecoratorTexturized(PostProcess *process, GLuint texture, GLuint programID, int width, int height);
	PostProcessDecoratorTexturized(PostProcess *process, GLuint texture, GLuint programID, GLuint outputBuffer);
	~PostProcessDecoratorTexturized();
	void PreProcess();
};

