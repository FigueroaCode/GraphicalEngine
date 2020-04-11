#include "headers/CommonGL.h"

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    bool noError = true;
    while (GLenum error = glGetError()) {
        printf("[Open GL error] (%d): %s in file: %s on line %d\n",
            error, function, file, line);
        noError = false;
    }
    return noError;
}