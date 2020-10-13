#include "Camera.h"



Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up, float yaw, float pitch, float movementSpeed, float mouseSensitivity, float zoom)
{
	this->position = position;
	this->forward = forward;
	this->up = up;
	this->right = normalize(glm::cross(forward, up));

	this->pitch = pitch;
	this->yaw = yaw;

	this->movementSpeed = movementSpeed;
	this->mouseSensitivity = mouseSensitivity;
	this->zoom = zoom;
}

glm::mat4 Camera::getViewMatrix()
{
	//lookAt takes our position, a target position (what our camera is supposed to look at), and a vector representing up in camera space
	return glm::lookAt(position, position + forward, up);	
}

void Camera::translate(CameraMovement camMovement, float deltaTime)
{
	glm::vec3 direction;

	switch (camMovement) {
	case CameraMovement::FORWARD:
		direction = forward;
		break;

	case CameraMovement::BACKWARD:
		direction = -forward;
		break;

	case CameraMovement::RIGHT:
		direction = right;
		break;

	case CameraMovement::LEFT:
		direction = -right;
		break;
	}

	position += direction * movementSpeed * deltaTime;
}
