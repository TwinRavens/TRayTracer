#include "gRayTracer.h"

int rav::RayTracer::height, rav::RayTracer::width, rav::RayTracer::depthLevel, rav::RayTracer::raysSize;
GLuint rav::RayTracer::screenBuffer, rav::RayTracer::raysBuffer, rav::RayTracer::rayHitsBuffer;
GLint rav::RayTracer::collisionProgram, rav::RayTracer::shadingProgram;
static int screenArea;
static Ray* rays;

GLint rav::RayTracer::generateRays()
{

#pragma region Screen Rays
	{

		//Define the number of rays by screen width and heigh
		screenArea = (width*height);
		raysSize = (width * height) * pow(2, depthLevel - 1);

#if ORTOGRAPHIC
		//Ortographic camera rays
		Ray* rays = new Ray[width*height];
		{
			int rayId = 0;
			float xStep = 2.0f / (width + 1), yStep = 2.0f / (height + 1);
			float xPos = -1.0f + xStep, yPos = -1.0f + yStep;
			for (size_t i = 0; i < height; i++)
			{
				for (size_t j = 0; j < width; j++)
				{
					rays[rayId].origin = { xPos, yPos, 0, 0 };
					rays[rayId].dirAndId = { 0, 0, -1, (float)rayId };
					rayId++;
					xPos += xStep;
				}
				xPos = -1.0f + xStep;
				yPos += yStep;
			}
		}
#elif false
		rays = new Ray[raysSize];
		{
			float aspectRatio = width / (float)height;
			float fov = 33; //get this from options
			float fovScale = tan(glm::radians(fov * 0.5));
			//glm::vec3 origin = { 0, 0, 0 };
			//cameraToWorld.multVecMatrix(Vec3f(0), origin);  
			glm_vec4 origin = { 0, 0, 20, 1 };

			int rayId = 0;
			for (size_t j = 0; j < height; j++)
			{
				for (size_t i = 0; i < width; i++)
				{
					float x = (2 * (i + 0.5) / (float)width - 1) * aspectRatio * fovScale;
					float y = (1 - 2 * (j + 0.5) / (float)height) * fovScale;
					glm::vec3 dir = { x, y, -1 };
					//cameraToWorld.multDirMatrix(Vec3f(x, y, -1), dir);
					dir = glm::normalize(dir);

					rays[rayId].origAndWght = origin;
					rays[rayId].dir = { dir.x, dir.y, dir.z };
					rays[rayId].pixelCoords = { i, j, 0, 0 };
					rayId++;
				}
			}
		}
#endif

		//Copy data to OpenGL
		glGenBuffers(1, &raysBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, raysBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Ray) * raysSize, NULL, GL_DYNAMIC_COPY);

#pragma region Collision Program Mapping
		{
			//Bind buffer base to mapping
			GLuint ssbo_binding_point_index = 1;
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, raysBuffer);

			//Get Shader Storage index from program
			GLuint block_index = 0;
			block_index = glGetProgramResourceIndex(collisionProgram, GL_SHADER_STORAGE_BLOCK, "rBuffer");
			if (block_index == GL_INVALID_INDEX)
			{
				rvDebug.Log("rBuffer Storage Block couldn't be found on program with id " + to_string(collisionProgram));

				//Return error code
				return 1;
			}
			rvDebug.Log("rBuffer Storage Block found at index " + to_string(block_index));

			//Associate buffer index with binding point
			glShaderStorageBlockBinding(collisionProgram, block_index, ssbo_binding_point_index);
			rvDebug.Log("rBuffer Storage Block binding is " + to_string(ssbo_binding_point_index));
		}
#pragma endregion

#pragma region Shading Program Mapping
		{
			//Bind buffer base to mapping
			GLuint ssbo_binding_point_index = 1;
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, raysBuffer);

			//Get Shader Storage index from program
			GLuint block_index = 0;
			block_index = glGetProgramResourceIndex(shadingProgram, GL_SHADER_STORAGE_BLOCK, "rBuffer");
			if (block_index == GL_INVALID_INDEX)
			{
				rvDebug.Log("rBuffer Storage Block couldn't be found on program with id " + to_string(shadingProgram));

				//Return error code
				return 1;
			}
			rvDebug.Log("rBuffer Storage Block found at index " + to_string(block_index));

			//Associate buffer index with binding point
			glShaderStorageBlockBinding(shadingProgram, block_index, ssbo_binding_point_index);
			rvDebug.Log("rBuffer Storage Block binding is " + to_string(ssbo_binding_point_index));
		}
#pragma endregion
	}
#pragma endregion

#pragma region RayHits
	{
		//Copy data to OpenGL
		glGenBuffers(1, &rayHitsBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, rayHitsBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(RayHit) * raysSize, NULL, GL_DYNAMIC_COPY);

#pragma region Collision Program Mapping
		{
			//Bind buffer base to mapping
			GLuint ssbo_binding_point_index = 2;
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, rayHitsBuffer);

			//Get Shader Storage index from program
			GLuint block_index = 0;
			block_index = glGetProgramResourceIndex(collisionProgram, GL_SHADER_STORAGE_BLOCK, "rHitBuffer");
			if (block_index == GL_INVALID_INDEX)
			{
				rvDebug.Log("rHitBuffer Storage Block couldn't be found on program with id " + to_string(collisionProgram));

				//Return error code
				return 1;
			}
			rvDebug.Log("rHitBuffer Storage Block found at index " + to_string(block_index));

			//Associate buffer index with binding point
			glShaderStorageBlockBinding(collisionProgram, block_index, ssbo_binding_point_index);
			rvDebug.Log("rHitBuffer Storage Block binding is " + to_string(ssbo_binding_point_index));
		}
#pragma endregion

#pragma region Shading Program Mapping
		{
			//Bind buffer base to mapping
			GLuint ssbo_binding_point_index = 2;
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, rayHitsBuffer);

			//Get Shader Storage index from program
			GLuint block_index = 0;
			block_index = glGetProgramResourceIndex(shadingProgram, GL_SHADER_STORAGE_BLOCK, "rHitBuffer");
			if (block_index == GL_INVALID_INDEX)
			{
				rvDebug.Log("rHitBuffer Storage Block couldn't be found on program with id " + to_string(shadingProgram));

				//Return error code
				return 1;
			}
			rvDebug.Log("rHitBuffer Storage Block found at index " + to_string(block_index));

			//Associate buffer index with binding point
			glShaderStorageBlockBinding(shadingProgram, block_index, ssbo_binding_point_index);
			rvDebug.Log("rHitBuffer Storage Block binding is " + to_string(ssbo_binding_point_index));
		}
#pragma endregion

	}
#pragma endregion

	return 0;
}

GLint rav::RayTracer::loadPrograms()
{

#pragma region Load Collision Pass Shader
	{
		//Load Compute Shader
		char* compute_shader;
		rvLoadFile("./data/collisionPass.comp", compute_shader, true);

		GLuint col_shader = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(col_shader, 1, &compute_shader, NULL);
		glCompileShader(col_shader);

		GLint isCompiled = 0;
		glGetShaderiv(col_shader, GL_COMPILE_STATUS, &isCompiled);

		//If got an error
		if (isCompiled == GL_FALSE)
		{
			//Get size of info log
			GLint maxLength = 0;
			glGetShaderiv(col_shader, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			GLchar *errorLog = new GLchar[maxLength];
			glGetShaderInfoLog(col_shader, maxLength, &maxLength, &errorLog[0]);

			//Error header
			rvDebug.Log("Error compiling collision pass shader ", RV_ERROR_MESSAGE);

			//Create message
			string msg((const char*)errorLog);

			//Pop last \0 char
			msg.pop_back();

			//Log Error through Debugger
			rvDebug.Log(msg, Debug::Error);

			//Don't leak the shader
			glDeleteShader(col_shader);

			//Return error code
			return 1;
		}
		else
		{
			rvDebug.Log("Sucessfully compiled collision pass shader!\n");
		}

		//Link program
		collisionProgram = glCreateProgram();
		glAttachShader(collisionProgram, col_shader);
		glLinkProgram(collisionProgram);
	}
#pragma endregion

#pragma region Load Shading Pass Shader
	{
		//Load Compute Shader
		char* compute_shader;
		rvLoadFile("./data/shadingPass.comp", compute_shader, true);

		GLuint shad_shader = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(shad_shader, 1, &compute_shader, NULL);
		glCompileShader(shad_shader);

		GLint isCompiled = 0;
		glGetShaderiv(shad_shader, GL_COMPILE_STATUS, &isCompiled);

		//If got an error
		if (isCompiled == GL_FALSE)
		{
			//Get size of info log
			GLint maxLength = 0;
			glGetShaderiv(shad_shader, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			GLchar *errorLog = new GLchar[maxLength];
			glGetShaderInfoLog(shad_shader, maxLength, &maxLength, &errorLog[0]);

			//Error header
			rvDebug.Log("Error compiling shading pass shader ", RV_ERROR_MESSAGE);

			//Create message
			string msg((const char*)errorLog);

			//Pop last \0 char
			msg.pop_back();

			//Log Error through Debugger
			rvDebug.Log(msg, Debug::Error);

			//Don't leak the shader
			glDeleteShader(shad_shader);

			//Return error code
			return 2;
		}
		else
		{
			rvDebug.Log("Sucessfully compiled shading shader!\n");
		}

		//Link program
		shadingProgram = glCreateProgram();
		glAttachShader(shadingProgram, shad_shader);
		glLinkProgram(shadingProgram);
	}
#pragma endregion

	//Return no error code
	return 0;
}

GLint rav::RayTracer::collisionPass(int depth_level)
{
	//Lunch Compute Shader!
	glUseProgram(collisionProgram);

	//Update time variable
	//GLint timeLoc = glGetUniformLocation(collisionProgram, "time");
	GLint timeLoc = 0;
	glUniform1f(timeLoc, glfwGetTime());

	//TODO: PASS THIS TO AN OBJECT MANAGER
	//GLint spheresCountId = glGetUniformLocation(collisionProgram, "spheresCount");
	GLint spheresCountId = 1;
	glUniform1i(spheresCountId, 7);

	//Update workgroup width variable
	//GLint groupWidthLoc = glGetUniformLocation(collisionProgram, "workGroupWidth");
	GLint groupWidthLoc = 2;
	glUniform1i(groupWidthLoc, width);

	//Update workgroup height variable
	//GLint groupHeightLoc = glGetUniformLocation(collisionProgram, "workGroupHeight");
	GLint groupHeightLoc = 3;
	glUniform1i(groupHeightLoc, height);

	//Update depth level
	//GLint depthLevelLoc = glGetUniformLocation(shadingProgram, "depth_level");
	GLint depthLevelLoc = 4;
	glUniform1i(depthLevelLoc, depth_level);

	//Dispatch compute shader to process
	glDispatchCompute(width * height * (depthLevel + 1) / 256, 1, 1);

	//Avoid concurrent memory access!
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	return 0;
}

GLint rav::RayTracer::shadingPass(int depth_level)
{
	//Lunch Compute Shader!
	glUseProgram(shadingProgram);

	//Bind front buffer to layout 0
	glBindImageTexture(0, screenBuffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	//Update time variable
	//GLint timeLoc = glGetUniformLocation(shadingProgram, "time");
	GLint timeLoc = 0;
	glUniform1f(timeLoc, glfwGetTime());

	//Update depth level
	//GLint depthLevelLoc = glGetUniformLocation(shadingProgram, "depthLevel");
	GLint depthLevelLoc = 1;
	glUniform1i(depthLevelLoc, depth_level);

	//Update workgroup width
	//GLint groupWidthLoc = glGetUniformLocation(shadingProgram, "workGroupWidth");
	GLint groupWidthLoc = 2;
	glUniform1i(groupWidthLoc, width);

	//Update screen area
	//GLint screenAreaLoc = glGetUniformLocation(shadingProgram, "screenArea");
	GLint screenAreaLoc = 3;
	glUniform1i(screenAreaLoc, screenArea);

	//Update ambient colour
	//GLint ambientColourLoc = glGetUniformLocation(shadingProgram, "ambientColour");
	GLint ambientColourLoc = 4;
	glUniform4f(ambientColourLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	//Dispatch compute shader to process
	glDispatchCompute(width * height / 32, (depthLevel + 1), 1);

	//Avoid concurrent memory access!
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	return 0;
}

GLint rav::RayTracer::Setup(int width, int height, int depth)
{
	//Hold texture size values
	rav::RayTracer::width = width;
	rav::RayTracer::height = height;
	rav::RayTracer::depthLevel = depth;

#pragma region Debug Compute Shader Variables
	//Get maximum compute capabilities
	int work_grp_cnt[3];

	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

	rvDebug.Log("Max global (total) work group size (" + std::to_string(work_grp_cnt[0]) +
		", " + std::to_string(work_grp_cnt[1]) + ", " + std::to_string(work_grp_cnt[2]) + ")");

	int work_grp_size[3];

	//
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

	rvDebug.Log("Max local (in one shader) work group size (" + std::to_string(work_grp_size[0]) +
		", " + std::to_string(work_grp_size[1]) + ", " + std::to_string(work_grp_size[2]) + ")");

	//
	int work_grp_inv;

	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	rvDebug.Log("Max local work group invocations " + std::to_string(work_grp_inv));

#pragma endregion

	//Load Compute Shaders
	if (GLint error = loadPrograms() != 0)
		return error;

	//Generate Rays (TODO: perspective camera)
	if (GLint error = generateRays() != 0)
		return error;

	//TODO: Move the objects handling to some resources manager
#pragma region Passing Buffers to Shader
	{

#pragma region Spheres
		{
			//Generate Primitives Information
			Sphere spheres[] = {
				//	  x		y	 z	  scale
				{	 13,	6,  -14,	1,	},
				{	-13,  -10,	-20,	3,	},
				{	 -5,	2,  -10,	1,	},
				{	  0,	0,  -15,	2,	},
				{	  7,	0,  -10,	1,	},
				{	 10,   -3,	 -5,	1,	},
				{	 11,   -3,	 -8,	1	}
			};

			//Copy data to OpenGL
			GLuint ssbo = 0;
			glGenBuffers(1, &ssbo);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(spheres), spheres, GL_STATIC_DRAW);

#pragma region Collision Pass Mapping
			{
				//Get Shader Storage index from program
				GLuint block_index = 0;
				block_index = glGetProgramResourceIndex(collisionProgram, GL_SHADER_STORAGE_BLOCK, "sBuffer");
				if (block_index == GL_INVALID_INDEX)
					rvDebug.Log("sBuffer Storage Block couldn't be found on collision program!");
				rvDebug.Log("sBuffer Storage Block found at index " + to_string(block_index));

				//Associate buffer index with binding point
				GLuint ssbo_binding_point_index = 3;

				glShaderStorageBlockBinding(collisionProgram, block_index, ssbo_binding_point_index);
				rvDebug.Log("sBuffer Storage Block binding is " + to_string(ssbo_binding_point_index));

				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, ssbo);

				//Define spheres count uniform
				GLint spheresCountId = glGetUniformLocation(collisionProgram, "spheresCount");
				glUniform1i(spheresCountId, sizeof(spheres) / sizeof(Sphere));
			}
#pragma endregion

		}
#pragma endregion

#pragma region Vertex
		{
			//Generate Primitives Information
			glm_vec4 vertex[] = {
				//	 x	 y	 z	padding
				{	-1,	-1,	-5,	1,	},
				{	 0,	 1,	-5,	1,	},
				{	 1,	-1,	-5,	1	}
			};

			//Copy data to OpenGL
			GLuint ssbo = 0;
			glGenBuffers(1, &ssbo);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

#pragma region Collision Pass Mapping
			{
				//Get Shader Storage index from program
				GLuint block_index = 0;
				block_index = glGetProgramResourceIndex(collisionProgram, GL_SHADER_STORAGE_BLOCK, "vBuffer");
				if (block_index == GL_INVALID_INDEX)
					rvDebug.Log("vBuffer Storage Block couldn't be found on collision program!");
				rvDebug.Log("vBuffer Storage Block found at index " + to_string(block_index));

				//Associate buffer index with binding point
				GLuint ssbo_binding_point_index = 4;

				glShaderStorageBlockBinding(collisionProgram, block_index, ssbo_binding_point_index);
				rvDebug.Log("vBuffer Storage Block binding is " + to_string(ssbo_binding_point_index));

				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, ssbo);
			}
#pragma endregion
		}
#pragma endregion

#pragma region Normals
		{
			//Generate Primitives Information
			glm_vec4 normals[] = {
				//	 x	 y	 z	padding
				{	 0,	 0,	 1,	1,	},
				{	 0,	 0,	 1,	1,	},
				{	 0,	 0,	 1,	1	}
			};

			//Copy data to OpenGL
			GLuint ssbo = 0;
			glGenBuffers(1, &ssbo);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

#pragma region Collision Pass Mapping
			{
				//Get Shader Storage index from program
				GLuint block_index = 0;
				block_index = glGetProgramResourceIndex(collisionProgram, GL_SHADER_STORAGE_BLOCK, "nBuffer");
				if (block_index == GL_INVALID_INDEX)
					rvDebug.Log("nBuffer Storage Block couldn't be found on collision program!");
				rvDebug.Log("nBuffer Storage Block found at index " + to_string(block_index));

				//Associate buffer index with binding point
				GLuint ssbo_binding_point_index = 5;

				glShaderStorageBlockBinding(collisionProgram, block_index, ssbo_binding_point_index);
				rvDebug.Log("nBuffer Storage Block binding is " + to_string(ssbo_binding_point_index));

				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, ssbo);
			}
#pragma endregion
		}
#pragma endregion

#pragma region Triangles
		{
			//Generate Primitives Information
			Triangle triangles[] = {
				//	1	2	3	padding
				{	0,	1,	2,	1	},	//Vertices
				{	0,	1,	2,	1	}	//Normals
			};

			//Copy data to OpenGL
			GLuint ssbo = 0;
			glGenBuffers(1, &ssbo);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

#pragma region Collision Pass Mapping
			{
				//Get Shader Storage index from program
				GLuint block_index = 0;
				block_index = glGetProgramResourceIndex(collisionProgram, GL_SHADER_STORAGE_BLOCK, "tBuffer");
				if (block_index == GL_INVALID_INDEX)
					rvDebug.Log("tBuffer Storage Block couldn't be found on collision program!");
				rvDebug.Log("tBuffer Storage Block found at index " + to_string(block_index));

				//Associate buffer index with binding point
				GLuint ssbo_binding_point_index = 6;

				glShaderStorageBlockBinding(collisionProgram, block_index, ssbo_binding_point_index);
				rvDebug.Log("tBuffer Storage Block binding is " + to_string(ssbo_binding_point_index));

				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, ssbo);
			}
#pragma endregion
		}
#pragma endregion

#pragma region Materials
		{

			Material materials[] = {
				//	red		green	blue	alpha	spec	diff	ambient	coef	shinness	refrac_index	reflection
				{	1,		0,		0,		1.0,	0.6,	0.8,	0.1,			50,			1.0,			0.0,	},
				{	0,		0.8,	0,		1.0,	0.6,	0.8,	0.1,			50,			1.0,			0.0,	},
				{	1,		0,		0.7,	1.0,	0.6,	0.8,	0.1,			50,			1.0,			0.3,	},
				{	0.7,	0.3,	0,		0.8,	0.6,	0.8,	0.1,			50,			1.0,			0.2,	},
				{	1,		0,		1,		1.0,	0.6,	0.8,	0.1,			50,			1.0,			0.6,	},
				{	0,		1,		1,		0.6,	0.9,	0.8,	0.1,			50,			1.05,			0.0,	},
				{	1,		1,		0,		1.0,	0.6,	0.2,	0.1,			50,			1.0,			0.8,	}
			};

			//Copy data to OpenGL
			GLuint ssbo = 0;
			glGenBuffers(1, &ssbo);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(materials), materials, GL_STATIC_DRAW);

#pragma region Shading Pass Mapping
			{
				//Get Shader Storage index from program
				GLuint block_index = 0;
				block_index = glGetProgramResourceIndex(shadingProgram, GL_SHADER_STORAGE_BLOCK, "sMatBuffer");
				if (block_index == GL_INVALID_INDEX)
					rvDebug.Log("sMatBuffer Storage Block couldn't be found on shading program!");
				rvDebug.Log("sMatBuffer Storage Block found at index " + to_string(block_index));

				//Associate buffer index with binding point
				GLuint ssbo_binding_point_index = 7;

				glShaderStorageBlockBinding(shadingProgram, block_index, ssbo_binding_point_index);
				rvDebug.Log("sMatBuffer Storage Block binding is " + to_string(ssbo_binding_point_index));

				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, ssbo);

			}
#pragma endregion

		}
#pragma endregion

#pragma region Lights
		{

			Light lights[] = {
			//x		y		z	count	r		g		b		a
			{5.0,	0.0,	2.0,	3,	1.0,	0.0,	0.0,	1.0 },
			{0.0,	-2.0,	0.0,	3,	0.0,	1.0,	0.0,	1.0},
			{-5.0,	1.0,	-1.0,	3,	1.0,	0.0,	1.0,	1.0 },
			};

			//Copy data to OpenGL
			GLuint ssbo = 0;
			glGenBuffers(1, &ssbo);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(lights), lights, GL_STATIC_DRAW);

#pragma region Shading Pass Mapping
			{
				//Get Shader Storage index from program
				GLuint block_index = 0;
				block_index = glGetProgramResourceIndex(shadingProgram, GL_SHADER_STORAGE_BLOCK, "sLightBuffer");
				if (block_index == GL_INVALID_INDEX)
					rvDebug.Log("sLightBuffer Storage Block couldn't be found on shading program!");
				rvDebug.Log("sLightBuffer Storage Block found at index " + to_string(block_index));

				//Associate buffer index with binding point
				GLuint ssbo_binding_point_index = 8;

				glShaderStorageBlockBinding(shadingProgram, block_index, ssbo_binding_point_index);
				rvDebug.Log("sLightBuffer Storage Block binding is " + to_string(ssbo_binding_point_index));

				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, ssbo);

			}
#pragma endregion

		}
#pragma endregion

	}
#pragma endregion

#pragma region Generate Screen Front Buffer
	//Generate empty texture
	glGenTextures(1, &screenBuffer);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	//Define texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//Repeat out of bounds UVs
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//Repeat out of bounds UVs
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//Set Image sampling filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//Set Image sampling filtering
#pragma endregion

	//Returns screen buffer index
	return screenBuffer;
}

GLint rav::RayTracer::Compute()
{

	for (size_t i = 0; i < depthLevel; i++)
	{

		collisionPass(i);

		shadingPass(i);

	}

	//Return OpenGL Front Buffer id
	return screenBuffer;
}
