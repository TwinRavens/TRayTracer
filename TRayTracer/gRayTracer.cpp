#include "gRayTracer.h"

int rav::RayTracer::height, rav::RayTracer::width, rav::RayTracer::raysSize;
GLuint rav::RayTracer::screenBuffer, rav::RayTracer::raysBuffer, rav::RayTracer::rayHitsBuffer;
GLint rav::RayTracer::collisionProgram, rav::RayTracer::shadingProgram;

GLint rav::RayTracer::generateRays()
{
#pragma region Input Rays
	{
		//Define the number of rays by screen width and heigh
		raysSize = width * height;

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
#else
		Ray* rays = new Ray[raysSize];
		{
			float aspectRatio = width / (float)height;
			float fov = 33; //get this from options
			float fovScale = tan(glm::radians(fov * 0.5));
			//glm::vec3 origin = { 0, 0, 0 };
			//cameraToWorld.multVecMatrix(Vec3f(0), origin);  
			glm_vec4 origin = { 0, 0, 20, 0.5 };

			int rayId = 0;
			for (size_t j = 0; j < height; j++)
			{
				for (size_t i = 0; i < width; i++)
				{
					float x = (2 * (i + 0.5) / (float)width - 1) * aspectRatio * fovScale;
					float y = (1 - 2 * (j + 0.5) / (float)height) * fovScale;
					glm::vec3 dir = { x,y,-1 };
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
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Ray) * raysSize, rays, GL_DYNAMIC_COPY);

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

#pragma region Output RayHits
	{
		RayHit* rayHits = new RayHit[raysSize];

		//Copy data to OpenGL
		glGenBuffers(1, &rayHitsBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, rayHitsBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(RayHit) * raysSize, rayHits, GL_DYNAMIC_COPY);

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

GLint rav::RayTracer::collisionPass()
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

	//Dispatch compute shader to process
	glDispatchCompute((GLuint)width, (GLuint)height, 1);

	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

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

	//Update ambient colour
	//GLint ambientColourLoc = glGetUniformLocation(shadingProgram, "ambientColour");
	GLint ambientColourLoc = 3;
	glUniform4f(ambientColourLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	//Dispatch compute shader to process
	glDispatchCompute((GLuint)width, (GLuint)height, 1);

	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	return 0;
}

GLint rav::RayTracer::Setup(int width, int height)
{
	//Hold texture size values
	rav::RayTracer::width = width;
	rav::RayTracer::height = height;

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
#pragma region Passing Primitives Buffer to Shader
	{
		//Generate Primitives Information
		Sphere spheres[] = {
			//POS x		y	 z	  scale		COL r	g	b	alpha	spec	diff	ambient	coef	shinness	refrac_index	reflection
			{	 13,	6,  -14,	1,			1,	0,	0,	1.0,	0.6,	0.8,	0.2,			50,			1.0,			0.3 },
			{	-13,  -10,  -20,	3,			1,	0,	0,	1.0,	0.6,	0.8,	0.2,			50,			1.0,			0.3 },
			{	 -5,	2,  -10,	1,			1,	0,	0,	1.0,	0.6,	0.8,	0.2,			50,			1.0,			0.3 },
			{	  0,	0,  -15,	2,			1,	0,	0,	1.0,	0.6,	0.8,	0.2,			50,			1.0,			0.3 },
			{	  7,	0,  -10,	1,			1,	0,	0,	1.0,	0.6,	0.8,	0.2,			50,			1.0,			0.3 },
			{	 10,   -3,	 -5,	1,			1,	0,	0,	1.0,	0.6,	0.8,	0.2,			50,			1.0,			0.3 },
			{	 11,   -3,	 -8,	1,			1,	0,	0,	1.0,	0.6,	0.8,	0.2,			50,			1.0,			0.3 }
		};

		GLint value = 0;
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &value);
		rvDebug.Log("Max Uniform Block size is " + to_string(value) + ", current spheres buffer size is " + to_string(sizeof(spheres)) + ".");
		rvDebug.Log("Max current Spheres count is " + to_string(value/sizeof(Sphere)) + ".");


		//Copy data to OpenGL
		GLuint ssbo = 0;
		glGenBuffers(1, &ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(spheres), spheres, GL_DYNAMIC_COPY);

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

#pragma region Shading Pass Mapping
		{
			//Get Shader Storage index from program
			GLuint block_index = 0;
			block_index = glGetProgramResourceIndex(shadingProgram, GL_SHADER_STORAGE_BLOCK, "sBuffer");
			if (block_index == GL_INVALID_INDEX)
				rvDebug.Log("sBuffer Storage Block couldn't be found on collision program!");
			rvDebug.Log("sBuffer Storage Block found at index " + to_string(block_index));

			//Associate buffer index with binding point
			GLuint ssbo_binding_point_index = 3;

			glShaderStorageBlockBinding(shadingProgram, block_index, ssbo_binding_point_index);
			rvDebug.Log("sBuffer Storage Block binding is " + to_string(ssbo_binding_point_index));

			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, ssbo);

			//Define spheres count uniform
			GLint spheresCountId = glGetUniformLocation(shadingProgram, "spheresCount");
			glUniform1i(spheresCountId, sizeof(spheres) / sizeof(Sphere));
		}
#pragma endregion

		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		//GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		//memcpy(p, rays, sizeof(sizeof(Ray) * width * height));
		//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


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

GLint rav::RayTracer::Compute(int depth_level)
{
	//clearPass(screenBuffer);
	
	for (size_t i = 0; i < depth_level; i++)
	{

		collisionPass();

		shadingPass(i);

	}

	//Return OpenGL Front Buffer id
	return screenBuffer;
}
