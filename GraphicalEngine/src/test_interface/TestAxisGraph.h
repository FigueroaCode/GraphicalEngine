#pragma once

#include "TestInterface.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"

#include "glm/glm.hpp"

namespace test {

	class TestAxisGraph : public TestInterface
	{
	public:
		TestAxisGraph();
		~TestAxisGraph();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		const int margin = 20;
		const int tickSize = 10;

		int window_width;
		int window_height;

		Shader shader;
		unsigned int box_vbo;
		unsigned int box_vao;
		unsigned int vbID;
		unsigned int vaID;

		float offset_x;
		float scale_x;
		glm::mat4 viewport_transform(float x, float y, float width, float height);
		void setupBox();
		void setupGraph();
		void drawGraph();
		void drawBox();
		void drawTicks();
	};
}