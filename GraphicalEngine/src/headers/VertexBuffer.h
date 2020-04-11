#pragma once

class VertexBuffer {
private:
	unsigned int rendererId;
public:
	VertexBuffer(const void* data, unsigned int size, unsigned int GLDrawStyle);
	~VertexBuffer();

	void setData(unsigned int offset, unsigned int size, const void* data);

	void bind() const;
	void unbind() const;
};