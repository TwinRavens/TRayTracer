#pragma once
#include "Primitives.h"


struct ObjectData {
	Triangle* triangles;
	glm::vec3* vertices;
	glm::vec3* normal;
	int verticesCount;
	int normalCount;
};