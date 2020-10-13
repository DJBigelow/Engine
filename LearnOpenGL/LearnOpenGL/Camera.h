#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = -90.0f,
			float pitch = 0.0f,
			float movementSpeed = 2.0f,
			float mouseSensitivity = 0.1f,
			float zoom = 45.0f);

	glm::mat4 getViewMatrix();

	void translate(CameraMovement camMovement, float deltaTime);
	void rotate(double xOffset, double yOffset);

private:
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	void updateDirectionVectors();
};