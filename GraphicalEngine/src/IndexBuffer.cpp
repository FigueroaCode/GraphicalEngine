#include "headers/IndexBuffer.h"
#include "headers/CommonGL.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));
	GLCheckError(glGenBuffers(1, &rendererId));
	setIndices(data, count);
}

IndexBuffer::~IndexBuffer() {
	GLCheckError(glDeleteBuffers(1, &rendererId));
}

void IndexBuffer::setIndices(const unsigned int* data, unsigned int count) {
	if (data != nullptr && count > 0) {
		this->count = count;
		GLCheckError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
		GLCheckError(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}
}

void IndexBuffer::bind() const {
	GLCheckError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
}

void IndexBuffer::unbind() const {
	GLCheckError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}