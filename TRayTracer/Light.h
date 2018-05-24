#pragma once
#include <GLM\glm.hpp>

struct Light 
{
	glm_vec4 lightPosCount; // light position (x,y,z) and count of how many light there is
	glm_vec4 lightColor; // light color (r, g, b, a = 1)
};