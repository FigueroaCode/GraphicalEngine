#include "headers/VertexArray.h"
#include "headers/CommonGL.h"

VertexArray::VertexArray() {
	GLCheckError(glGenVertexArrays(1, &rendererId));
}

VertexArray::~VertexArray() {
	GLCheckError(glDeleteVertexArrays(1, &rendererId));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		GLCheckError(glEnableVertexAttribArray(i));
		// Handles geometry of buffer points
		// params: (attrId, coordinate space size{right now in 2D},
		// attr type, whether or not to normalize data{turn data into range of 0->1},
		// how many bytes until the next point, offset between attributes in bytes{ex:offsetof(vertexStruct)})
		GLCheckError(glVertexAttribPointer(i, element.count, element.type,
			element.normalized, layout.getStride(), (const void*) offset));

		offset += element.count * BufferLayoutElement::getSizeByType(element.type);
	}
}

void VertexArray::bind() const {
	GLCheckError(glBindVertexArray(rendererId));
}

void VertexArray::unbind() const {
	GLCheckError(glBindVertexArray(0));
}