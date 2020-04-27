#include "TestAxisGraph.h"
#include "CommonGL.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/gtc/matrix_transform.hpp"

struct pointA {
	float x;
	float y;
};

namespace test {

	TestAxisGraph::TestAxisGraph()
		:TestInterface("Axis Graph"),
		shader("resources/shaders/axis.shader"),
		offset_x(0),
		scale_x(1),
		window_width(1024),
		window_height(512) {
		GLCheckError(glEnable(GL_BLEND));
		GLCheckError(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		// Won't work on window resize, if we want that to happen then the window size needs to be updated in the render function
		//glViewport(
		//	margin + tickSize, // x offset in pixels from lower left of window
		//	margin + tickSize, // y offset
		//	window_width - margin * 2 - tickSize, // width of viewport in pixels
		//	window_height - margin * 2 - tickSize // height of viewport in pixels
		//);
		// Make sure clipping happens
		//glScissor(
		//	margin + tickSize,
		//	margin + tickSize,
		//	window_width - margin * 2 - tickSize,
		//	window_height - margin * 2 - tickSize
		//);
		//glEnable(GL_SCISSOR_TEST);
		
		setupBox();

		setupGraph();
	}

	TestAxisGraph::~TestAxisGraph() {

	}

	void TestAxisGraph::onUpdate(float deltaTime) {

	}

	void TestAxisGraph::onRender() {

		GLCheckError(glClearColor(0.1f, 0.2f, 0.3f, 1.0f));
		GLCheckError(glClear(GL_COLOR_BUFFER_BIT));

		
		// Won't work on window resize, if we want that to happen then the window size needs to be updated in the render function
		glViewport(
			margin + tickSize, // x offset in pixels from lower left of window
			margin + tickSize, // y offset
			window_width - margin * 2 - tickSize, // width of viewport in pixels
			window_height - margin * 2 - tickSize // height of viewport in pixels
		);
		// Make sure clipping happens
		glScissor(
			margin + tickSize,
			margin + tickSize,
			window_width - margin * 2 - tickSize,
			window_height - margin * 2 - tickSize
		);
		glEnable(GL_SCISSOR_TEST);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(scale_x, 1, 1));
		glm::mat4 transform = glm::translate(scale, glm::vec3(offset_x, 0, 0));
		shader.bind();
		shader.setUniformMat4f("transform", transform);
		drawGraph();

		// Don't clip Box
		glViewport(0, 0, window_width, window_height);
		glDisable(GL_SCISSOR_TEST);
		// Add viewport effect to box
		transform = viewport_transform(
			margin + tickSize,
			margin + tickSize,
			window_width - margin * 2 - tickSize,
			window_height - margin * 2 - tickSize
		);

		shader.bind();
		shader.setUniformMat4f("transform", transform);
		drawBox();

		drawTicks();
	}

	void TestAxisGraph::onImGuiRender() {
		ImGui::SliderFloat("Offset X", &offset_x, -10, 10);
		ImGui::SliderFloat("Scale X", &scale_x, 1, 10);
	}

	void TestAxisGraph::setupGraph() {
		pointA graph[2000];
		for (int i = 0; i < 2000; i++) {
			float x = (i - 1000.0f) / 100.0f;
			graph[i].x = x;
			graph[i].y = sin(x * 10.0f) / (1.0f + x * x);
		}

		
		GLCheckError(glGenBuffers(1, &vbID));
		GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, vbID));
		GLCheckError(glBufferData(GL_ARRAY_BUFFER, sizeof(graph), graph, GL_STATIC_DRAW));

		GLCheckError(glGenVertexArrays(1, &vaID));
		GLCheckError(glBindVertexArray(vaID));

		GLCheckError(glEnableVertexAttribArray(0));
		GLCheckError(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));
	}

	void TestAxisGraph::setupBox() {
		GLCheckError(glGenBuffers(1, &box_vbo));
		GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, box_vbo));

		static const pointA box[4] = { {-1, -1}, {1, -1}, {1, 1}, {-1, 1} };
		GLCheckError(glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW));

		//GLfloat black[4] = { 0, 0, 0, 1 };
		//glUniform4fv(uniform_color, 1, black);
		GLCheckError(glGenVertexArrays(1, &box_vao));
		GLCheckError(glBindVertexArray(box_vao));

		GLCheckError(glEnableVertexAttribArray(0));
		GLCheckError(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));
	}

	void TestAxisGraph::drawGraph() {
		GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, vbID));
		GLCheckError(glBindVertexArray(vaID));

		GLCheckError(glDrawArrays(GL_LINE_STRIP, 0, 2000));
		//GLCheckError(glDrawArrays(GL_POINTS, 0, 2000));
	}

	void TestAxisGraph::drawBox() {
		GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, box_vbo));
		GLCheckError(glBindVertexArray(box_vao));

		GLCheckError(glDrawArrays(GL_LINE_LOOP, 0, 4));
	}

	void TestAxisGraph::drawTicks() {
		float pixel_x = 2.0 / (window_width - margin * 2 - tickSize);
		float pixel_y = 2.0 / (window_height - margin * 2 - tickSize);
		GLuint ticks_vbo;
		GLCheckError(glGenBuffers(1, &ticks_vbo));
		GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, ticks_vbo));

		pointA yticks[42];

		for (int i = 0; i <= 20; i++) {
			float y = -1 + i * 0.1;
			yticks[i * 2].x = -1;
			yticks[i * 2].y = y;
			float tickscale = (i % 10) ? 0.5 : 1;
			yticks[i * 2 + 1].x = -1 - tickSize * tickscale * pixel_x;
			yticks[i * 2 + 1].y = y;
		}

		GLCheckError(glBufferData(GL_ARRAY_BUFFER, sizeof yticks, yticks, GL_STREAM_DRAW));

		unsigned int ticks_vao;
		GLCheckError(glGenVertexArrays(1, &ticks_vao));
		GLCheckError(glBindVertexArray(ticks_vao));

		GLCheckError(glEnableVertexAttribArray(0));

		GLCheckError(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));
		GLCheckError(glDrawArrays(GL_LINES, 0, 42));

		// X axis ticks
		float tickspacing = 0.1 * powf(10, -floor(log10(scale_x)));

		float left = -1.0 / scale_x - offset_x;
		float right = 1.0 / scale_x - offset_x;

		int left_i = ceil(left / tickspacing);
		int right_i = floor(right / tickspacing);

		// Difference between left border and left most tick in graph units
		float rem = left_i * tickspacing - left;

		float firsttick = -1.0 + rem * scale_x;

		int nticks = right_i - left_i + 1;
		if (nticks > 21)
			nticks = 21;

		pointA xticks[42];

		for (int i = 0; i < nticks; i++) {
			float x = firsttick + i * tickspacing * scale_x;
			float tickscale = ((i + left_i) % 10) ? 0.5 : 1;
			float y = -1 - tickSize * tickscale * pixel_y;
			xticks[i * 2].x = x;
			xticks[i * 2].y = -1;
			xticks[i * 2 + 1].x = x;
			xticks[i * 2 + 1].y = y;
		}

		GLCheckError(glBufferData(GL_ARRAY_BUFFER, sizeof(xticks), xticks, GL_STREAM_DRAW));

		GLCheckError(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));
		GLCheckError(glDrawArrays(GL_LINES, 0, nticks * 2));
	}

	glm::mat4 TestAxisGraph::viewport_transform(float x, float y, float width, float height) {
		// Calculate how to translate the x and y coordinates:
		float offsetX = (2.0 * x + (width - window_width)) / window_width;
		float offsetY = (2.0 * y + (height - window_height)) / window_height;

		// Calculate how to rescale the x and y coordinates:
		float scaleX = width / window_width;
		float scaleY = height / window_height;

		return glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(offsetX, offsetY, 0)), glm::vec3(scaleX, scaleY, 1));
	}
}