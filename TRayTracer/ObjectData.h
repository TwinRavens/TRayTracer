#pragma once
#include "Primitives.h"


struct ObjectData {
	Triangle* triangles;
	glm::vec4* vertices;
	glm::vec4* normal;
	int verticesCount = 0;
	int triangleCount = 0;
};