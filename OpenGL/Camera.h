#pragma once
#include"includes.h"

class Camera
{
public:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(0.0, 1.0, 0.0), cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	glm::mat4 view = glm::lookAt(glm::vec3(camX, camY, camZ), cameraTarget, cameraUp);

	float cameraSpeed = 0.002f;

	float radiusXZ = 0;
	float camX, camY, camZ;

	Camera();
	Camera(glm::mat4* view);
	glm::mat4 setRotateXZ(float radius);
	glm::mat4 enableMove(bool enable);
	glm::mat4 Move(unsigned int way);
};