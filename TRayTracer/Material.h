#pragma once
#include <GLM\glm.hpp>

struct Material
{
	glm_vec4 diffColAlpha;				//Difuse color and refraction coeficient (transparency)
	glm_vec4 diffSpecAmbiCoefShinness;	//Difuse, Specular and Ambient Coeficient and Shinness power
	glm_vec4 refrIndexReflCoef;			//Refraction Index, comReflaction Coeficient and two padding
};
