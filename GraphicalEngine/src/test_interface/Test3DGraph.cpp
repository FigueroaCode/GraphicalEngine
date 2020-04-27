#include "Test3DGraph.h"
#include "CommonGL.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
struct pointB {
	float x;
	float y;
};

namespace test {

	Test3DGraph::Test3DGraph()
		:TestInterface("3D Graph"),
		shader("resources/shaders/graph3d.shader"),
		offset_x(0),
		scale_x(1),
		offset_y(0),
		scale_y(1),
		window_width(1024),
		window_height(512) {
		GLCheckError(glEnable(GL_BLEND));
		GLCheckError(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		setupGraph();
	}

	Test3DGraph::~Test3DGraph() {

	}

	void Test3DGraph::onUpdate(float deltaTime) {

	}

	void Test3DGraph::onRender() {
		GLCheckError(glClearColor(0.1f, 0.2f, 0.3f, 1.0f));
		GLCheckError(glClear(GL_COLOR_BUFFER_BIT));

		glm::mat4 texture_transform = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(scale_x, scale_y, 1)), glm::vec3(offset_x, offset_y, 0));

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0, -2.0, 2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 projection = glm::perspective(45.0f, 1.0f * 640 / 480, 0.1f, 10.0f);

		glm::mat4 vertex_transform = projection * view * model;

		shader.bind();
		shader.setUniformMat4f("texture_transform", texture_transform);
		shader.setUniformMat4f("vertex_transform", vertex_transform);
		//shader.setUniform1i("myTexture", 0);

		drawGraph();
	}

	void Test3DGraph::onImGuiRender() {
		ImGui::SliderFloat("Offset X", &offset_x, -10, 10);
		ImGui::SliderFloat("Scale X", &scale_x, 1, 10);

		ImGui::SliderFloat("Offset Y", &offset_y, -10, 10);
		ImGui::SliderFloat("Scale Y", &scale_y, 1, 10);
	}

	void Test3DGraph::setupGraph() {
		// Shader was not getting values from Texture for some reason, so just implemented the function in the shader
		//for (int i = 0; i < N; i++) {
		//	for (int j = 0; j < N; j++) {
		//		float x = (i - N / 2) / (N / 2.0);
		//		float y = (j - N / 2) / (N / 2.0);
		//		float t = hypotf(x, y) * 4.0;
		//		float z = (1 - t * t) * expf(t * t / -2.0);
		//		graph[i][j] = roundf(z * 127 + 128);
		//		//graph[i][j] = 0;
		//		//std::cout << graph[i][j] << std::endl;
		//	}
		//}
		///* Set texture wrapping mode */
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		///* Set texture interpolation mode */
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//// Store points in texture
		//GLCheckError(glActiveTexture(GL_TEXTURE0));
		//GLCheckError(glGenTextures(1, &texture_id));
		//GLCheckError(glBindTexture(GL_TEXTURE_2D, texture_id));
		//GLCheckError(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, N, N, 0, GL_RED, GL_UNSIGNED_BYTE, graph));

		// Setup grid lines
		pointB vertices[101][101];

		for (int i = 0; i < 101; i++) {
			for (int j = 0; j < 101; j++) {
				vertices[i][j].x = (j - 50) / 50.0;
				vertices[i][j].y = (i - 50) / 50.0;
			}
		}
		GLCheckError(glGenBuffers(1, &grid_vbo));
		GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, grid_vbo));
		GLCheckError(glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW));

		GLushort indices[2 * 100 * 101 * 2];
		int i = 0;

		// Horizontal grid lines
		for (int y = 0; y < 101; y++) {
			for (int x = 0; x < 100; x++) {
				indices[i++] = y * 101 + x;
				indices[i++] = y * 101 + x + 1;
			}
		}

		// Vertical grid lines
		for (int x = 0; x < 101; x++) {
			for (int y = 0; y < 100; y++) {
				indices[i++] = y * 101 + x;
				indices[i++] = (y + 1) * 101 + x;
			}
		}

		GLCheckError(glGenBuffers(1, &grid_ibo));
		GLCheckError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid_ibo));
		GLCheckError(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW));

		GLCheckError(glGenVertexArrays(1, &grid_vao));
		GLCheckError(glBindVertexArray(grid_vao));

		GLCheckError(glEnableVertexAttribArray(0));
		GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, grid_vbo));
		GLCheckError(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));
	}

	void Test3DGraph::drawGraph() {
		// Draw grid
		GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, grid_vbo));
		GLCheckError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid_ibo));
		GLCheckError(glBindVertexArray(grid_vao));
		GLCheckError(glDrawElements(GL_LINES, 2 * 100 * 101 * 2, GL_UNSIGNED_SHORT, 0));
	}
}