#pragma once

#include <glm/glm.hpp>

namespace DG
{
	class Camera
	{
	public:
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		void setPosition(glm::vec3 position) { this->position = position; }
		void move(glm::vec3 direction, float deltaTime);
		void processMouse(double xPos, double yPos);

		glm::mat4 getViewMatrix() const;
		glm::vec3 getPosition() const { return position; }
		glm::vec3 getFront() const { return front; }
		glm::vec3 getRight() const { return right; }

		static const float YAW;
		static const float PITCH;
		static const float SPEED;
		static const float SENSITIVITY;
		static const glm::vec3 WORLDUP;
	private:
		void updateVectors();

		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;

		float yaw;
		float pitch;
		float sensitivity;

		bool firstMouse = true;
		double lastX = 0.0;
		double lastY = 0.0;
	};
}