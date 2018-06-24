#include "Object.h"


Object::Object(ObjectData * data)
{
	this->data = data;
	//copy vertices from data
	modVertices = new glm::vec3[data->verticesCount];
	for (int i = 0; i < data->verticesCount; i++) {
		modVertices[i] = data->vertices[i];
	}

	transformMat = mat4x4(1);
	needUpdate = false;
}

void Object::UpdateVertices()
{
	if (needUpdate)
	{
	glm:vec4 temp(1);
		for (int i = 0; i < data->verticesCount; i++)
		{
			temp.x = data->vertices[i].x;
			temp.y = data->vertices[i].y;
			temp.z = data->vertices[i].z;
			modVertices[i] = transformMat * temp;
		}
		needUpdate = false;
	}
}


Object& Object::Translate(vec3 pos)
{
	needUpdate = true;
	transformMat = glm::translate(transformMat, pos);
	return *this;
}

Object& Object::Rotate(float angle, vec3 axis)
{
	needUpdate = true;
	transformMat = glm::rotate(transformMat, angle, axis);
	return *this;
}


Object& Object::Scale(vec3 scale)
{
	needUpdate = true;
	transformMat = glm::scale(transformMat, scale);
	return *this;
}

vec3 * Object::getVertices()
{
	UpdateVertices();
	return modVertices;
}

