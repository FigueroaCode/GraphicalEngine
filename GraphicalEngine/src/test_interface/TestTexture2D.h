#pragma once

#include "TestInterface.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"

#include <memory>

namespace test {
	struct Vertex {
		glm::vec4 Position;
		float TexCoords[2];
		float TexId;
	};

	class TestTexture2D : public TestInterface {
	public:
		TestTexture2D();
		~TestTexture2D();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		std::unique_ptr <VertexArray> vao;
		std::unique_ptr <VertexBuffer> vb;
		std::unique_ptr <IndexBuffer> ib;
		std::unique_ptr <Shader> shader;
		std::unique_ptr <Texture> texture;
		std::unique_ptr <Texture> texture2;

		float translationA[3];
		float rotation;
		glm::vec3 translationB;

		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;

		std::array<Vertex, 8> createQuad(float size, float x, float y, float z, float texId);
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


