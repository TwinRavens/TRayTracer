#pragma once
#include <GLM\glm.hpp>

struct Material
{
	glm_vec4 color;						//RGBA with A beeing refraction coeficient
	glm_vec4 difSpecAmbiCoefShinness;	//Difuse, specular and ambient coeficients and shinness strength
	glm_vec4 refrIndexReflCoef;			//Refraction Index and reflection coeficients
};
