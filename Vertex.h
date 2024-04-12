//Christopher Rode
//Date: 3/31/24
//version 2.1
//this code sets our vec3 positions, colors, and normals for our VBO

#pragma once
#include <glm\glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
};
