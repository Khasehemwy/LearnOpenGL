#include "Window.h"

void Window::frameBuffer_size_callback(GLFWwindow* window, int width, int heigth)
{
	glViewport(0, 0, width, heigth);
}
Window::Window(float width, float height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(width, height, title, NULL, NULL);
	this->width = width;
	this->height = height;

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBuffer_size_callback);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

