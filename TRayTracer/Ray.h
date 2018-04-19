#pragma once
#include <GLM\glm.hpp>

struct Ray
{
	glm_vec4 origAndWght;	//Ray origin position and weight
	glm_vec4 dirAndId;		//Direction and pixel ID
};

struct RayHit
{
	glm_vec4 hitAndWeight;	//Hit position and color weight
	glm_vec4 norAndId;		//Normal ind pixel ID
	int objId, objTypeId;	//Id of Object that got hit
	float p1, p2;			//Memory Padding
};