#pragma once

#include "../engine/camera.hpp"
#include <glm/glm.hpp>

struct GLFWwindow;

class Player
{
public:
	Player() : camera{}, position{ 0.0f, 0.0f, 0.0f } {};
	Player(glm::vec3 position) : camera{ position + glm::vec3(0.0f, 0.5f, 0.0f) }, position{ position } {};
	void setPosition(glm::vec3 position);
	void update(GLFWwindow* window, float deltaTime);

	DG::Camera camera;
private:
	glm::vec3 position;
	float speed = 2.5f;
};