#include "Camera.h"

Camera::Camera()
{
	camX = cameraPos.x;
	camY = cameraPos.y;
	camZ = cameraPos.z;
}

Camera::Camera(glm::mat4* view)
{

}

glm::mat4 Camera::setRotateXZ(float radius)
{
	this->radiusXZ = radius;
	glm::mat4 view(1.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return view;
}

glm::mat4 Camera::enableMove(bool enable)
{
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return view;
}

glm::mat4 Camera::Move(unsigned int way)
{
	switch (way)
	{
	case 1:
		cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraUp, glm::cross(cameraFront, cameraUp)));
		break;
	case 2:
		cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraUp, glm::cross(cameraFront, cameraUp)));
		break;
	case 3:
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	case 4:
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	default:
		break;
	}
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	this->view = view;
	return view;
}
