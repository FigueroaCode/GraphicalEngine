#include "TestTexture2D.h"

#include "CommonGL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

#include <array>

namespace test {
	TestTexture2D::TestTexture2D()
        : translationA{ 0.0f, 0.0f, -4.0f },
          rotation(0.0f),
          translationB(150.0f, 200.0f, 0.0f),
          proj(glm::perspective(recalculatefov(), 1.0f * 1024 / 512, 0.1f, 10.0f)),
          view(glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0))),
          model(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
          TestInterface("Texture 2D")
    {

        // Needed to remove duplicate vertices
        // Clock wise indices
        unsigned int indices[] = {
            // Front
            0, 3, 1,
            3, 2, 1,
            // Back
            6, 4, 7,
            4, 5, 7,
            // Right
            1, 2, 7,
            2, 5, 7,
            // Left
            0, 3, 6,
            3, 4, 6,
            // Top
            3, 4, 2,
            4, 5, 2,
            // Bottom
            0, 6, 1,
            6, 7, 1
        };

        //glClearDepth(1.0f);                   // Set background depth to farthest
        glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
        glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
        glShadeModel(GL_SMOOTH);   // Enable smooth shading
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

        vao = std::make_unique<VertexArray>();
        ib = std::make_unique<IndexBuffer>(indices, 6 * 6);

        vb = std::make_unique<VertexBuffer>(nullptr, 16 * sizeof(Vertex), GL_DYNAMIC_DRAW);
        VertexBufferLayout layout;
        // Position
        layout.push<float>(4);
        // Texture
        layout.push<float>(2);
        // Texture id
        layout.push<float>(1);
        vao->addBuffer(*vb, layout);

        glm::mat4 mvp = proj * view * model;
        /* Setup shaders */
        std::string png = "resources/textures/transparentTex.png";
        std::string jpg = "resources/textures/greyCopperRustyTex.jpg";
        texture = std::make_unique<Texture>(jpg);
        //texture->bind(0);
        texture2 = std::make_unique<Texture>(png);
        //texture2->bind(1);

        shader = std::make_unique<Shader>("resources/shaders/basic.shader");
        shader->bind();

        int samples[2] = {0, 1};
        shader->setUniform1iv("u_Textures", 2, samples);
        shader->setUniformMat4f("u_ModelViewProjection", mvp);
	}

	TestTexture2D::~TestTexture2D() {

	}

    std::array<Vertex, 8> TestTexture2D::createQuad(float size, float x, float y, float z, float texId) {

        glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

        Vertex v0;
        v0.Position = { x, y, z, 1.0f };
        v0.Position = transform * v0.Position;
        v0.TexCoords[0] = 0.0f;
        v0.TexCoords[1] = 0.0f;
        v0.TexId = texId;

        Vertex v1;
        v1.Position = { x + size, y, z, 1.0f };
        v1.Position = transform * v1.Position;
        v1.TexCoords[0] = 0.5f;
        v1.TexCoords[1] = 0.0f;
        v1.TexId = texId;

        Vertex v2;
        v2.Position = { x + size, y + size, z, 1.0f };
        v2.Position = transform * v2.Position;
        v2.TexCoords[0] = 1.0f;
        v2.TexCoords[1] = 1.0f;
        v2.TexId = texId;

        Vertex v3;
        v3.Position = { x, y + size, z, 1.0f };
        v3.Position = transform * v3.Position;
        v3.TexCoords[0] = 0.0f;
        v3.TexCoords[1] = 0.5f;
        v3.TexId = texId;

        Vertex v4;
        v4.Position = { x, y + size, z + size, 1.0f };
        v4.Position = transform * v4.Position;
        v4.TexCoords[0] = 0.0f;
        v4.TexCoords[1] = 0.0f;
        v4.TexId = texId;

        Vertex v5;
        v5.Position = { x + size, y + size, z + size, 1.0f };
        v5.Position = transform * v5.Position;
        v5.TexCoords[0] = 0.5f;
        v5.TexCoords[1] = 0.0f;
        v5.TexId = texId;

        Vertex v6;
        v6.Position = { x, y, z + size, 1.0f };
        v6.Position = transform * v6.Position;
        v6.TexCoords[0] = 1.0f;
        v6.TexCoords[1] = 1.0f;
        v6.TexId = texId;

        Vertex v7;
        v7.Position = { x + size, y, z + size, 1.0f };
        v7.Position = transform * v7.Position;
        v7.TexCoords[0] = 0.0f;
        v7.TexCoords[1] = 0.5f;
        v7.TexId = texId;

        return { v0, v1, v2, v3, v4, v5, v6, v7 };
    }

    void TestTexture2D::onUpdate(float deltaTime) {

	}

	void TestTexture2D::onRender() {
        // Set dynamic vertex buffer
        /* Setup Vertices */
        //float positions[] = {
        //    //( windowX, windowY, textureX, textureY, textureId)
        //   -50.0f, -50.0f, 0.0f, 0.0f, 0.0f, // 0
        //    50.0f, -50.0f, 0.5f, 0.0f, 0.0f, // 1
        //    50.0f,  50.0f, 1.0f, 1.0f, 0.0f, // 2
        //   -50.0f,  50.0f, 0.0f, 0.5f, 0.0f, // 3

        //    50.0f, -50.0f, 0.0f, 0.0f, 1.0f, // 4
        //    150.0f,-50.0f, 0.5f, 0.0f, 1.0f, // 5
        //    150.0f, 50.0f, 1.0f, 1.0f, 1.0f, // 6
        //    50.0f,  50.0f, 0.0f, 0.5f, 1.0f  // 7
        //};

        auto quad0 = createQuad(1.0f, translationA[0], translationA[1], translationA[2], 0.0f);
        //auto quad1 = createQuad(100.0f, translationB.x, translationB.y, translationB.z, 1.0f);

        // TODO: Turn vertex into class, rotate just the cube
        Vertex vertices[8];
        memcpy(vertices, quad0.data(), quad0.size() * sizeof(Vertex));
        //memcpy(vertices + quad0.size(), quad1.data(), quad1.size() * sizeof(Vertex));

        vb->setData(0, sizeof(vertices), vertices);

		GLCheckError(glClearColor(0.2f, 0.3f, 0.7f, 1.0f));
		GLCheckError(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;
        texture->bind(0);
        //texture2->bind(1);

        glm::mat4 mvp = proj * view * model;
        shader->bind();
        shader->setUniformMat4f("u_ModelViewProjection", mvp);
        renderer.draw(*vao, *ib, *shader);
	}

	void TestTexture2D::onImGuiRender() {
        ImGui::DragFloat3("A", translationA, 0.8f, -10.0f, 10.0f);
        //ImGui::DragFloat("Rotation", &rotation, 0.8f, 0.0f, 360.0f);
        //ImGui::SliderFloat3("TranslationB", &translationB.x, 0.0f, 1024);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}