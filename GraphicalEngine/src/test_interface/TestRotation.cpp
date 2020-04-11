#include "TestRotation.h"

#include "CommonGL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

#include <array>

namespace test {
    TestRotation::TestRotation()
        :rotation{ 0.0f, 0.0f, 0.0f },
         cube(0.5f),
         cubeColor{ 0.0f, 0.0f, 0.0f, 1.0f },
         proj(glm::perspective(recalculatefov(), 1.0f * 1024 / 512, 0.1f, 10.0f)),
         model(glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.8f, -4.0f))),
         TestInterface("Rotations"){

        // Set how to handle blending situation for textures
        GLCheckError(glEnable(GL_BLEND));
        GLCheckError(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        //glClearDepth(1.0f);                   // Set background depth to farthest
        glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
        glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
        glShadeModel(GL_SMOOTH);   // Enable smooth shading
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
    }

    void TestRotation::onUpdate(float deltaTime) {
        rotation[0] += deltaTime / ImGui::GetFrameCount();
        rotation[1] += deltaTime / ImGui::GetFrameCount();
        rotation[2] += deltaTime / ImGui::GetFrameCount();
    }

    void TestRotation::onRender() {
        Renderer renderer;
        float sideLength = 0.5f;
        float positions[] = {
            // front
            -sideLength, -sideLength,  sideLength, 1.0,
             sideLength, -sideLength,  sideLength, 1.0,
             sideLength,  sideLength, sideLength, 1.0,
            -sideLength,  sideLength,  sideLength, 1.0,
            // back
            -sideLength, -sideLength, -sideLength, 1.0,
            sideLength, -sideLength, -sideLength, 1.0,
             sideLength, sideLength, -sideLength, 1.0,
            -sideLength,  sideLength, -sideLength, 1.0
        };

        GLCheckError(glClearColor(0.2f, 0.3f, 0.7f, 1.0f));
        renderer.clear();
        cube.setVertices(positions, cubeColor);
        //glm::mat4 projMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        cube.applyMvp(proj * model * rotateZ * rotateY * rotateX);

        renderer.draw(*cube.getVertexArray(), *cube.getIndexBuffer(), *cube.getShader());
    }

    TestRotation::~TestRotation() {

    }

    void TestRotation::onImGuiRender() {
        ImGui::SliderFloat3("Rotations", rotation, -360.0f, 360.0f);
        ImGui::ColorEdit3("Cube color", cubeColor);
        //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}