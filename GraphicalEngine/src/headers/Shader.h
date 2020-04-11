#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct vec4 {
	float x;
	float y;
	float z;
	float w;
};

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	unsigned int rendererId;
	std::string filepath;
	// caching for uniforms
	std::unordered_map<std::string, int> uniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	// Set uniforms
	void setUniform1iv(const std::string& name, int count, int* samples);
	void setUniform1i(const std::string& name, int value);
	void setUniform1f(const std::string& name, float value);
	void setUniform4f(const std::string& name, vec4 position);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int compileShader(unsigned int type, const std::string& source);
	ShaderProgramSource parseShader();

	int getUniformLocation(const std::string& name);
};