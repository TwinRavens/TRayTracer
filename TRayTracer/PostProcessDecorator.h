#pragma once
#include "PostProcess.h"
class PostProcessDecorator : public PostProcess
{
protected:
	void Setup(PostProcess *process);
	PostProcess * postProcess;
public:
	PostProcessDecorator(PostProcess *process, GLuint vertexShader, GLuint fragmentShader, GLuint outputBuffer);
	PostProcessDecorator(PostProcess *process, GLuint vertexShader, GLuint fragmentShader, int width, int height);
	PostProcessDecorator(PostProcess *process, GLuint programID, int width, int height);
	PostProcessDecorator(PostProcess *process, GLuint programID, GLuint outputBuffer);
	~PostProcessDecorator();

	GLuint Process(GLuint input);

};

