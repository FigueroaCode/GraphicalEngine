#include "headers/Shader.h"
#include "headers/CommonGL.h"

#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath)
: filepath(filepath), rendererId(0) {
    ShaderProgramSource source = parseShader();
    rendererId = createShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
    GLCheckError(glDeleteProgram(rendererId));
}

void Shader::bind() const {
    GLCheckError(glUseProgram(rendererId));
}

void Shader::unbind() const {
    GLCheckError(glUseProgram(0));
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLCheckError(unsigned int programId = glCreateProgram());
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCheckError(glAttachShader(programId, vs));
    GLCheckError(glAttachShader(programId, fs));
    GLCheckError(glLinkProgram(programId));
    GLCheckError(glValidateProgram(programId));

    GLCheckError(glDeleteShader(vs));
    GLCheckError(glDeleteShader(fs));

    return programId;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    GLCheckError(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    // (shader id, pointer ref to string containing shader code,
    // length{if null, assumes string is null terminated})
    GLCheckError(glShaderSource(id, 1, &src, nullptr));
    GLCheckError(glCompileShader(id));

    int result;
    GLCheckError(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCheckError(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCheckError(glGetShaderInfoLog(id, length, &length, message));
        std::string vertexType = (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
        printf("Failed to compile %s shader:\n %s\n", vertexType.c_str(), message);
        GLCheckError(glDeleteShader(id));
        return 0;
    }


    return id;
}

ShaderProgramSource Shader::parseShader() {
    std::ifstream inputStream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(inputStream, line)) {
        if (line.find("#shader") != std::string::npos) {
            // Found Shader
            if (line.find("vertex") != std::string::npos) {
                // Found vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                // Found fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            int i = (int)type;
            ss[i] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

void Shader::setUniform1iv(const std::string& name, int count, int* samples) {
    GLCheckError(glUniform1iv(getUniformLocation(name), count, samples));
}

void Shader::setUniform1i(const std::string& name, int value) {
    GLCheckError(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1f(const std::string& name, float value) {
    GLCheckError(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string& name, vec4 position) {
    GLCheckError(glUniform4f(getUniformLocation(name), position.x, position.y, position.z, position.w));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLCheckError(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::getUniformLocation(const std::string& name) {
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
        return uniformLocationCache[name];

    GLCheckError(int location = glGetUniformLocation(rendererId, name.c_str()));
    if (location == -1)
        printf("Warning: uniform %s doesn't exit!\n", name.c_str());

    uniformLocationCache[name] = location;

    return location;
}