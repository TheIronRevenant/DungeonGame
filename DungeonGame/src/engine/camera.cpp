#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include <algorithm>
#include <iostream>

const float DG::Camera::YAW = -90.f;
const float DG::Camera::PITCH = 0.0f;
const float DG::Camera::SPEED = 2.5f;
const float DG::Camera::SENSITIVITY = 0.1f;
const glm::vec3 DG::Camera::WORLDUP{ 0.0f, 1.0f, 0.0f };

DG::Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: position{ position }, front{ glm::vec3(0.0f, 0.0f, -1.0f) }, yaw{ yaw }, pitch{ pitch }, speed{ SPEED }, sensitivity{ SENSITIVITY }
{
	updateVectors();
}

glm::mat4 DG::Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

void DG::Camera::move(glm::vec3 direction, float deltaTime)
{
	if (freeMove)
	{
		direction = glm::normalize(direction) * speed * deltaTime;
		position += front * direction.z;
		position += right * direction.x;
	}
	else
	{
		glm::vec3 lockedFront = front;
		glm::vec3 lockedRight = right;
		lockedFront.y = 0;
		lockedRight.y = 0;
		lockedFront = glm::normalize(lockedFront);
		lockedRight = glm::normalize(lockedRight);

		direction = glm::normalize(direction) * speed * deltaTime;
		position += lockedFront * direction.z;
		position += lockedRight * direction.x;
	}
}

void DG::Camera::processMouse(double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = (float)(xPos - lastX) * sensitivity;
	float yOffset = (float)(lastY - yPos) * sensitivity;

	lastX = xPos;
	lastY = yPos;

	yaw += xOffset;
	pitch += yOffset;
	pitch = std::clamp(pitch, -89.0f, 89.0f);

	updateVectors();
}

void DG::Camera::updateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = glm::normalize(front);
	right = glm::normalize(glm::cross(this->front, WORLDUP));
	up = glm::normalize(glm::cross(right, this->front));
}