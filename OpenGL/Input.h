#pragma once
#include"includes.h"

class Input
{
private:
	Window* window;
	Shader* shader;
	Camera* camera;
public:
	Input(Window* window, Shader* shader,Camera* camera);
	void EnableCursor();
	void EnableScroll(float* fov);
	void ProcessInput();
};