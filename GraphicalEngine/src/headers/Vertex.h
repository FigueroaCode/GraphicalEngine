#pragma once

#include "glm/glm.hpp"

class Vertex
{
public:
	glm::vec4 position;
	glm::vec4 color;
	Vertex()
		: position(0.0f, 0.0f, 0.0f, 0.0f), color(0.0f, 0.0f, 0.0f, 1.0f) {}
};

