#include "headers/Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	:rendererId(0), filepath(path), localBuffer(nullptr),
	width(0), height(0), BPP(0){
	// Opengl coordinates start on bot-left, but png start on top-right
	// so a flip is necessary
	stbi_set_flip_vertically_on_load(1);
	unsigned int channels = getChannelCount(path);
	localBuffer = stbi_load(path.c_str(), &width, &height, &BPP, channels); // 4 - rgba; 3 - rgb

	GLCheckError(glGenTextures(1, &rendererId));
	GLCheckError(glBindTexture(GL_TEXTURE_2D, rendererId));

	// NECESSARY
	GLCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); // Horizontal
	GLCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); // vertical

	// GL_RGBA8 for 4 channels for first GL_RGB, for second use GL_RGBA
	if (channels == 4) {
		GLCheckError(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
	}
	else {
		GLCheckError(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, localBuffer));
	}

	unbind();

	if (localBuffer)
		stbi_image_free(localBuffer);
}

Texture::~Texture() {
	GLCheckError(glDeleteTextures(1, &rendererId));
}

unsigned int Texture::getChannelCount(const std::string& path) const{
	std::size_t periodIndex = path.find_last_of(".");
	if (periodIndex != std::string::npos) {
		std::string extension = path.substr(periodIndex + 1);
		if (extension == "png") {
			return 4;
		}
		else {
			return 3;
		}
	}
	else {
		printf("Incorrect file format.\n");
		return 3;
	}
}

void Texture::bind(unsigned int slot) const{
	GLCheckError(glActiveTexture(GL_TEXTURE0 + slot));
	GLCheckError(glBindTexture(GL_TEXTURE_2D, rendererId));
}

void Texture::unbind() const {
	GLCheckError(glBindTexture(GL_TEXTURE_2D, 0));
}