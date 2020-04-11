#pragma once

#include "CommonGL.h"

class Texture {
private:
	unsigned int rendererId;
	std::string filepath;
	unsigned char* localBuffer;
	int width, height, BPP; // Bits per pixel
public:
	Texture(const std::string& path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	unsigned int getChannelCount(const std::string& path) const;

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
};