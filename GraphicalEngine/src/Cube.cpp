#include "Cube.h"
#include "glm/gtc/matrix_transform.hpp"

Cube::Cube(float sideLength)
	: sideLength(sideLength) {
    createCube();
}

Cube::~Cube() {
    delete vertexBuffer;
    delete vertexArray;
    delete indexBuffer;
    delete shader;
}

void Cube::createCube() {
    // Clock wise indices
    unsigned int indices[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3
    };

    vertexArray = new VertexArray();
    indexBuffer = new IndexBuffer(indices, 36);
    vertexBuffer = new VertexBuffer(nullptr , VERTEX_COUNT * sizeof(Vertex), GL_DYNAMIC_DRAW);
    VertexBufferLayout layout;
    // Position
    layout.push<float>(4);
    // Color
    layout.push<float>(4);
    vertexArray->addBuffer(*vertexBuffer, layout);

    //setVertices(positions, colors);
    shader = new Shader("resources/shaders/cube.shader");
}

void Cube::applyMvp(glm::mat4 mvp) {
    shader->bind();
    shader->setUniformMat4f("u_mvp", mvp);
}

void Cube::setVertices(float* positions, float* colors) {

    vertices[0].position = { positions[0], positions[1], positions[2], positions[3]     };
    vertices[1].position = { positions[4], positions[5], positions[6], positions[7]     };
    vertices[2].position = { positions[8], positions[9], positions[10], positions[11]   };
    vertices[3].position = { positions[12], positions[13], positions[14], positions[15] };
    vertices[4].position = { positions[16], positions[17], positions[18], positions[19] };
    vertices[5].position = { positions[20], positions[21], positions[22], positions[23] };
    vertices[6].position = { positions[24], positions[25], positions[26], positions[27] };
    vertices[7].position = { positions[28], positions[29], positions[30], positions[31] };

    vertices[0].color = { colors[0],  colors[1],  colors[2],  colors[3] };
    vertices[1].color = { colors[0],  colors[1],  colors[2],  colors[3] };
    vertices[2].color = { colors[0],  colors[1],  colors[2],  colors[3] };
    vertices[3].color = { colors[0],  colors[1],  colors[2],  colors[3] };
    vertices[4].color = { colors[0],  colors[1],  colors[2],  colors[3] };
    vertices[5].color = { colors[0],  colors[1],  colors[2],  colors[3] };
    vertices[6].color = { colors[0],  colors[1],  colors[2],  colors[3] };
    vertices[7].color = { colors[0],  colors[1],  colors[2],  colors[3] };

    //vertices[0].color = { colors[0],  colors[1],  colors[2],  colors[3] };
    //vertices[1].color = { colors[4],  colors[5],  colors[6],  colors[7] };
    //vertices[2].color = { colors[8],  colors[9],  colors[10], colors[11] };
    //vertices[3].color = { colors[12], colors[13], colors[14], colors[15] };
    //vertices[4].color = { colors[16], colors[17], colors[18], colors[19] };
    //vertices[5].color = { colors[20], colors[21], colors[22], colors[23] };
    //vertices[6].color = { colors[24], colors[25], colors[26], colors[27] };
    //vertices[7].color = { colors[28], colors[29], colors[30], colors[31] };

    //vertices[0].position = { -sideLength, -sideLength,  sideLength, 1.0 };
    //vertices[1].position = { sideLength, -sideLength,  sideLength, 1.0 };
    //vertices[2].position = { sideLength,  sideLength, sideLength, 1.0 };
    //vertices[3].position = { -sideLength,  sideLength,  sideLength, 1.0 };
    //vertices[4].position = { -sideLength, -sideLength, -sideLength, 1.0 };
    //vertices[5].position = { sideLength, -sideLength, -sideLength, 1.0 };
    //vertices[6].position = { sideLength, sideLength, -sideLength, 1.0 };
    //vertices[7].position = { -sideLength,  sideLength, -sideLength, 1.0 };
    //                       
    //vertices[0].color = { 1.0, 0.0, 0.0, 1.0 };
    //vertices[1].color = { 0.0, 1.0, 0.0, 1.0 };
    //vertices[2].color = { 0.0, 0.0, 1.0, 1.0 };
    //vertices[3].color = { 1.0, 1.0, 1.0, 1.0 };
    //vertices[4].color = { 1.0, 0.0, 0.0, 1.0 };
    //vertices[5].color = { 0.0, 1.0, 0.0, 1.0 };
    //vertices[6].color = { 0.0, 0.0, 1.0, 1.0 };
    //vertices[7].color = { 1.0, 1.0, 1.0, 1.0 };
    vertexBuffer->setData(0, sizeof(vertices), vertices);
}