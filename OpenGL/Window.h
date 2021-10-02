#pragma once
#include "includes.h"

class Window
{
private:
	static void frameBuffer_size_callback(GLFWwindow* window, int width, int heigth);

public:
	GLFWwindow* window;
	float width = 1400, height = 800;

	Window(float width, float height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
};

