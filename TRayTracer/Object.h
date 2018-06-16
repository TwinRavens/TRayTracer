#pragma once
#include <array>

#include "Primitives.h"



class Object {
private:
	Triangle* triangles;
	glm::vec3* rawVertices;
	glm::vec3* rawNormal;
	glm::vec3* modVertices;
	int verticesCount;
	
public:
	Object();


};