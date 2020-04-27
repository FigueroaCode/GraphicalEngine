#include "TestTextureGraph.h"
#include "CommonGL.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/gtc/matrix_transform.hpp"

namespace test {

	TestTextureGraph::TestTextureGraph()
		:TestInterface("Texture Graph"),
		shader("resources/shaders/Graph.shader"),
		offset_x(0),
		scale_x(1) {
		GLCheckError(glEnable(GL_BLEND));
		GLCheckError(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Create static x coordinates
		unsigned int vbX_ID;
		GLCheckError(glGenBuffers(1, &vbX_ID));
		GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, vbX_ID));

		// 101 x coordinates that go from -1 to 1
		float x_points[101];

		for (int i = 0; i < 101; i++) {
			x_points[i] = (i - 50) / 50.0f;
		}

		GLCheckError(glBufferData(GL_ARRAY_BUFFER, sizeof(x_points), x_points, GL_STATIC_DRAW));
		
		unsigned int va_ID;
		GLCheckError(glGenVertexArrays(1, &va_ID));
		GLCheckError(glBindVertexArray(va_ID));

		GLCheckError(glEnableVertexAttribArray(0));
		GLCheckError(glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0));

		// Map y points to color to store them as a texture
		unsigned char y_points[2048];

		for (int i = 0; i < 2048; i++) {
			float x = (i - 1024.0) / 100.0;
			float y = sin(x * 10.0) / (1.0 + x * x);
			y_points[i] = roundf(y * 128 + 128);
		}

		GLCheckError(glActiveTexture(GL_TEXTURE0));
		unsigned int yTex_ID;
		GLCheckError(glGenTextures(1, &yTex_ID));
		GLCheckError(glBindTexture(GL_TEXTURE_2D, yTex_ID));

		GLCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));

		GLCheckError(glTexImage2D(
			GL_TEXTURE_2D,      // target
			0,                  // level, 0 = base, no minimap,
			GL_RED,       // internalformat (using only 1 color component)
			2048,               // width
			1,                  // height
			0,                  // border, always 0 in OpenGL ES
			GL_RED,       // format
			GL_UNSIGNED_BYTE,   // type
			y_points
		));
	}

	TestTextureGraph::~TestTextureGraph() {

	}

	void TestTextureGraph::onUpdate(float deltaTime) {

	}

	void TestTextureGraph::onRender() {

		GLCheckError(glClearColor(0.2f, 0.3f, 0.7f, 1.0f));
		GLCheckError(glClear(GL_COLOR_BUFFER_BIT));

		shader.bind();
		shader.setUniform1f("offset_x", offset_x);
		shader.setUniform1f("scale_x", scale_x);
		shader.setUniform1i("yTexture", 0);

		GLCheckError(glDrawArrays(GL_LINE_STRIP, 0, 101));
	}

	void TestTextureGraph::onImGuiRender() {
		ImGui::SliderFloat("Offset X", &offset_x, -10, 10);
		ImGui::SliderFloat("Scale X", &scale_x, 0, 10);
	}
}