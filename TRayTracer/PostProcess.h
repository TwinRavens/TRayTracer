#pragma once
#include "gVertexBuffer.h"
#include "gVertexArray.h"

class PostProcess
{
protected:
	//rect
	GLuint program;
	GLuint outputBuffer;
	GLuint FBO;
	rav::VertexArray* screenQuadVAO = nullptr;
	rav::VertexBuffer* screenQuadVBO = nullptr;
	void SetupProgram(GLuint vertexShader, GLuint fragmentShader);
	void CreateOutputBuffer(int width, int height);
	void CreateFBO();
public:
	PostProcess(GLuint vertexShader, GLuint fragmentShader, GLuint outputBuffer);
	PostProcess(GLuint vertexShader, GLuint fragmentShader, int width, int height);
	PostProcess(GLuint programID, int width, int height);
	PostProcess(GLuint programID, GLuint outputBuffer);
	~PostProcess();

	virtual GLuint Process(GLuint input);

	void CreateScreenQuad();
	void setScreenQuad(rav::VertexArray* screenQuadVAO, rav::VertexBuffer* screenQuadVBO);
	void getScreenQuad(rav::VertexArray** screenQuadVAO, rav::VertexBuffer** screenQuadVBO);
};

