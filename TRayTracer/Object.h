#pragma once
#include "ObjectData.h"
#include <GLM\gtx\transform.hpp>

using namespace glm;

class Object {
private:
	//Verticed that are transformed
	vec3* modVertices;
	ObjectData *data;
	glm::mat4 transformMat;
	bool needUpdate;
	void UpdateVertices();

public:
	Object(ObjectData *data);

	Object& Translate(vec3 pos);
	Object& Rotate(float angle, vec3 axis);
	Object& Scale(vec3 scale);

	vec3* getVertices();

};