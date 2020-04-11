#pragma once

#include <vector>
#include <functional>
#include <string>
#include <iostream>

namespace test {
	class TestInterface {
	public:
		TestInterface(const std::string testName)
			:testName(testName) {}
		virtual  ~TestInterface() {}

		virtual void onUpdate(float deltaTime) {}
		virtual void onRender() {}
		virtual void onImGuiRender() {}

		inline std::string getTestName() const { return testName; }
	protected:
		std::string testName;
	};

	class TestMenu : public TestInterface {
	public:
		TestMenu(TestInterface*& currentTestPointer);
		~TestMenu();

		void onImGuiRender() override;
		template<typename T>
		void registerTest(const std::string& testName) {
			std::cout << "Registering test " << testName << std::endl;
			tests.push_back(std::make_pair(testName, []() { return new T(); }));
		}

	private:
		TestInterface*& currentTest;
		std::vector< std::pair<std::string, std::function<TestInterface* ()> > > tests;
	};
}