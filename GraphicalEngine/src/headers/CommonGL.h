#pragma once

#include <GL/glew.h>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCheckError(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)); //#x turns function name into a string

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);
