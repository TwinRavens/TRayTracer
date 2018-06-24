#include "PostProcessDecoratorTexturized.h"




PostProcessDecoratorTexturized::PostProcessDecoratorTexturized(PostProcess *process, GLuint texture, GLuint vertexShader, GLuint fragmentShader, GLuint outputBuffer) :
	PostProcessDecorator(process, vertexShader, fragmentShader, outputBuffer)
{
	this->texture = texture;
}

PostProcessDecoratorTexturized::PostProcessDecoratorTexturized(PostProcess *process, GLuint texture, GLuint vertexShader, GLuint fragmentShader, int width, int height) :
	PostProcessDecorator(process, vertexShader, fragmentShader, width, height)
{
	this->texture = texture;
}

PostProcessDecoratorTexturized::PostProcessDecoratorTexturized(PostProcess *process, GLuint texture, GLuint programID, int width, int height) : PostProcessDecorator(process, programID, width, height)
{
	this->texture = texture;
}

PostProcessDecoratorTexturized::PostProcessDecoratorTexturized(PostProcess *process, GLuint texture, GLuint programID, GLuint outputBuffer) : PostProcessDecorator(process, programID, outputBuffer)
{
	this->texture = texture;
}

PostProcessDecoratorTexturized::~PostProcessDecoratorTexturized()
{

}

void PostProcessDecoratorTexturized::PreProcess()
{
	GLint screen = glGetUniformLocation(program, "screen");
	glProgramUniform1i(program, screen, 0);

	GLint other = glGetUniformLocation(program, "other");
	glProgramUniform1i(program, other, 1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
}





