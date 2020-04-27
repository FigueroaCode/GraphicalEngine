#pragma once

#include "TestInterface.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"

#include "glm/glm.hpp"

#define N 256

namespace test {

	class Test3DGraph : public TestInterface
	{
	public:
		Test3DGraph();
		~Test3DGraph();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		GLubyte  graph[N][N];
		unsigned int texture_id;

		const int margin = 20;
		const int tickSize = 10;

		int window_width;
		int window_height;

		Shader shader;
		unsigned int grid_vbo;
		unsigned int grid_ibo;
		unsigned int grid_vao;
		unsigned int vbID;
		unsigned int vaID;

		float offset_x;
		float scale_x;
		float offset_y;
		float scale_y;

		void setupGraph();
		void drawGraph();
	};
}