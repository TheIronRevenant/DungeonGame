#include "player.hpp"

#include <GLFW/glfw3.h>

void Player::setPosition(glm::vec3 position)
{
	this->position = position;
	camera.setPosition(position);
}

void Player::update(GLFWwindow* window, float deltaTime)
{
	int forward = 0;
	int side = 0;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		forward += 1;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		forward -= 1;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		side -= 1;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		side += 1;

	glm::vec3 move((float)side, 0.0f, (float)forward);

	if (move != glm::vec3(0.0f))
	{
		move = glm::normalize(move) * speed * deltaTime;

		glm::vec3 lockedFront = camera.getFront();
		glm::vec3 lockedRight = camera.getRight();
		lockedFront.y = 0;
		lockedRight.y = 0;
		lockedFront = glm::normalize(lockedFront);
		lockedRight = glm::normalize(lockedRight);

		position += lockedFront * move.z;
		position += lockedRight * move.x;

		camera.setPosition(position);
	}
}