#pragma once

#include "headers/CommonGL.h"
#include <GLFW/glfw3.h>

class GLWindow {
private:
	float width;
	float height;
	GLFWwindow* window;
public:
	GLWindow(float width, float height);
	~GLWindow();

	bool init();
};