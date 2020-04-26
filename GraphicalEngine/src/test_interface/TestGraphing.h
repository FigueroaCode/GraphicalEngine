#pragma once

#include "TestInterface.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"

#include "glm/glm.hpp"

#define POINT_COUNT 2000

namespace test {
	struct point {
		float x;
		float y;
	};

	class TestGraphing : public TestInterface
	{
	public:
		TestGraphing();
		~TestGraphing();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		point graph[POINT_COUNT];
		Shader shader;

		float offset_x;
		float scale_x;
	};
}