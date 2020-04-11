#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer {
private:

public:
	Renderer();
	void clear() const;
	void draw(const VertexArray& va, const IndexBuffer& id, const Shader& shader) const;
};