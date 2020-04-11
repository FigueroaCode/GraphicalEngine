#include "TestInterface.h"
#include "imgui/imgui.h"

namespace test {
	TestMenu::TestMenu(TestInterface*& currentTestPointer) 
		:currentTest(currentTestPointer),
		TestInterface("Menu") {}

	TestMenu::~TestMenu() {

	}

	void TestMenu::onImGuiRender() {
		for (auto& test : tests) {
			// If button is pressed, call functions
			if (ImGui::Button(test.first.c_str())) {
				currentTest = test.second();
			}
		}
	}
}