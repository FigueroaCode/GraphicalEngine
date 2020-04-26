#include "TestGraphing.h"
#include "CommonGL.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/gtc/matrix_transform.hpp"

namespace test {

	TestGraphing::TestGraphing()
		:TestInterface("Graphing"),
		 shader("resources/shaders/line.shader"),
		 offset_x(0),
		 scale_x(1) {
		GLCheckError(glEnable(GL_BLEND));
		GLCheckError(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		//GLCheckError(glEnable(GL_POINT_SPRITE));
		//GLCheckError(glEnable(GL_VERTEX_PROGRAM_POINT_SIZE));

		for (int i = 0; i < POINT_COUNT; i++) {
			float x = (i - 1000.0f) / 100.0f;
			graph[i].x = x;
			graph[i].y = sin(x * 10.0f) / (1.0f + x * x);
		}

		unsigned int vbID;
		GLCheckError(glGenBuffers(1, &vbID));
		GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, vbID));
		GLCheckError(glBufferData(GL_ARRAY_BUFFER, sizeof(graph), graph, GL_STATIC_DRAW));

		unsigned int vaID;
		GLCheckError(glGenVertexArrays(1, &vaID));
		GLCheckError(glBindVertexArray(vaID));

		GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, vbID));

		GLCheckError(glEnableVertexAttribArray(0));
		GLCheckError(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));
	}

	TestGraphing::~TestGraphing() {

	}

	void TestGraphing::onUpdate(float deltaTime) {

	}

	void TestGraphing::onRender() {

		GLCheckError(glClearColor(0.2f, 0.3f, 0.7f, 1.0f));
		GLCheckError(glClear(GL_COLOR_BUFFER_BIT));

		shader.bind();
		shader.setUniform1f("offset_x", offset_x);
		shader.setUniform1f("scale_x", scale_x);

		GLCheckError(glDrawArrays(GL_LINE_STRIP, 0, 2000));
		//GLCheckError(glDrawArrays(GL_POINTS, 0, 2000));
	}

	void TestGraphing::onImGuiRender() {
		ImGui::SliderFloat("Offset X", &offset_x, -10, 10);
		ImGui::SliderFloat("Scale X", &scale_x, 0, 10);
	}
}