#pragma once
#include <GLM\glm.hpp>

struct Sphere
{
	glm_vec4 posAndScale;	 
	glm_vec4 diffColAndCoef; 
	glm_vec4 specPowAndCoef; //And Two padding floats
};

typedef Sphere UniformCube;
