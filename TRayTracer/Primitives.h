#pragma once
#include <GLM\glm.hpp>

struct Sphere
{
	glm_vec4 posAndScale;
	glm_vec4 diffColAlpha;
	glm_vec4 specDiffAmbiCoefShinness;
	glm_vec4 refrIndexReflCoef; //And two padding
};
