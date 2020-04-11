#pragma once

class IndexBuffer {
private:
	unsigned int rendererId;
	unsigned int count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void setIndices(const unsigned int* data, unsigned int count);

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return this->count; }
};