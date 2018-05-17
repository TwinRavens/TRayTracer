#pragma once
#include <GLM\glm.hpp>

struct Ray
{
	glm_vec4 origAndWght;				//Ray origin position and weight
	glm_vec4 dir;						//Direction and padding
	glm::ivec4 pixelCoords;				//Pixel coordinates
};

struct RayHit
{
	glm_vec4 hitAndWeight;				//Hit position and color weight
	glm_vec4 normal;					//Normal ind id of the object that got hit
	glm_vec4 rayDir;					//The viewing direction of this hit
	glm::ivec4 pixelCoordsObjIdType;	//Screen pixel coordinates, id and type of object that got hit
};
		 