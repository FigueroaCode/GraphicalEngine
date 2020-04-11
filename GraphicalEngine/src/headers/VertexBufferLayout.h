#pragma once

#include <GL/glew.h>
#include <vector>

struct BufferLayoutElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeByType(unsigned int type) {
		switch (type) {
			case GL_FLOAT:
				return 4;
			case GL_UNSIGNED_INT:
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
		}
		// Unsupported type given
		__debugbreak();
		return 0;
	}
};

class VertexBufferLayout {
private:
	std::vector<BufferLayoutElement> elements;
	unsigned int stride;
public:
	VertexBufferLayout()
		: stride(0) {}

	template<typename T>
	void push(unsigned int count) {
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count) {
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += count * sizeof(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count) {
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += count * sizeof(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count) {
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += count * sizeof(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<BufferLayoutElement> getElements() const { return elements; }
	inline unsigned int getStride() const { return stride; }
};