#pragma once
#include <GLM\glm.hpp>

struct Ray
{
	glm_vec4 origin;	//Ray start position
	glm_vec4 dirAndId;	//Direction and packed ID
};

struct RayHit
{
	glm_vec4 hit;		//Hit position
	glm_vec4 norAndId;	//Normal ind packed ID
};