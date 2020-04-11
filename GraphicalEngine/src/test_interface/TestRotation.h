#pragma once

#include "TestInterface.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"

#include "glm/glm.hpp"

#include "Cube.h"

#include <memory>

namespace test {
	class TestRotation : public TestInterface
	{
	public:
		TestRotation();
		~TestRotation();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		Cube cube;
		float cubeColor[4];
		float rotation[3];
		//std::unique_ptr <VertexArray> vao;
		//std::unique_ptr <VertexBuffer> vb;
		//std::unique_ptr <IndexBuffer> ib;
		//std::unique_ptr <Shader> shader;

		glm::mat4 proj;
		//glm::mat4 view;
		glm::mat4 model;

		inline float recalculatefov() {
			return 2.0f * glm::atan(glm::tan(glm::radians(45.0f / 2.0f)) / aspectaxis());
		}

		inline float aspectaxis()
		{
			float outputzoom = 1.0f;
			float aspectorigin = 16.0f / 9.0f;
			int aspectconstraint = 1;
			int screen_width = 1024;
			int screen_height = 512;
			switch (aspectconstraint)
			{
			case 1:
				if ((screen_width / screen_height) < aspectorigin)
				{
					outputzoom *= ((screen_width / screen_height) / aspectorigin);
				}
				else
				{
					outputzoom *= (aspectorigin / aspectorigin);
				}
				break;
			case 2:
				outputzoom *= ((screen_width / screen_height) / aspectorigin);
				break;
			default:
				outputzoom *= (aspectorigin / aspectorigin);
			}
			return outputzoom;
		}
	};
}


