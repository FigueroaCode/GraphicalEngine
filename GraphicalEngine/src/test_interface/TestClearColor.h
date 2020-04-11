#pragma once

#include "TestInterface.h"

namespace test {
	class TestClearColor : public TestInterface {
	public:
		TestClearColor();
		~TestClearColor();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		float clearColor[4];
	};
}


