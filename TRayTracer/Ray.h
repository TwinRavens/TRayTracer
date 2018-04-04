#pragma once
#include <GLM\glm.hpp>

struct RayInput
{
	glm_vec4 origin;
	glm_vec4 dir;
	uint32_t pixel_id;
};

struct RayHit
{
	glm_vec4 hit;
	glm_vec4 normal;
	uint32_t pixel_id;
};