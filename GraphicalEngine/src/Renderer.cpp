#include "headers/Renderer.h"
#include "headers/CommonGL.h"

Renderer::Renderer() {
}

void Renderer::clear() const {
    GLCheckError(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.bind();
    va.bind();
    ib.bind();
    // nullptr: indices are already in element array buffer, so no need to provide them
    // alternatively we can pass in indices, and not make the element array buffer
    GLCheckError(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}
