#pragma once

#include "CommonGL.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Vertex.h"

#include "glm/glm.hpp"

#define VERTEX_COUNT 8
class Cube
{
private:
	VertexBuffer* vertexBuffer;
	VertexArray* vertexArray;
	IndexBuffer* indexBuffer;
	Shader* shader;

	Vertex vertices[VERTEX_COUNT];
	float sideLength;
public:
	Cube(float sideLength);
	~Cube();

	void createCube();

	void setVertices(float* positions, float* colors);

	void applyMvp(glm::mat4 mvp);

	inline VertexArray* getVertexArray() { return vertexArray; };
	inline IndexBuffer* getIndexBuffer() { return indexBuffer; };
	inline Shader* getShader() { return shader; };
};

