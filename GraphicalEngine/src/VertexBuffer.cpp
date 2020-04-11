#include "headers/VertexBuffer.h"
#include "headers/CommonGL.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int GLDrawStyle) {
	GLCheckError(glGenBuffers(1, &rendererId));
	GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
	GLCheckError(glBufferData(GL_ARRAY_BUFFER, size, data, GLDrawStyle));
}

VertexBuffer::~VertexBuffer() {
	GLCheckError(glDeleteBuffers(1, &rendererId));
}

void VertexBuffer::setData(unsigned int offset, unsigned int size,  const void* data) {
	bind();
	GLCheckError(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void VertexBuffer::bind() const {
	GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
}

void VertexBuffer::unbind() const {
	GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, 0));
}