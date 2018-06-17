#pragma once
#include <GLM\glm.hpp>

struct Sphere
{
	glm_vec4 posAndScale;
};

struct Triangle
{
	unsigned int v1, v2, v3, _padding1;
	unsigned int n1, n2, n3, _padding2;
};
