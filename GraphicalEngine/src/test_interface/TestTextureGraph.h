#pragma once

#include "TestInterface.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"

#include "glm/glm.hpp"

struct point {
	float x;
	float y;
};

namespace test {

	class TestTextureGraph : public TestInterface
	{
	public:
		TestTextureGraph();
		~TestTextureGraph();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		Shader shader;

		float offset_x;
		float scale_x;
	};
}