#pragma once

#include "opengl/shader.hpp"
#include "scene.hpp"
#include "opengl/renderer.hpp"

#include <glm/glm.hpp>

#include <string>
#include <vector>

struct GLFWwindow;

namespace DG
{
	class GameObject;

	struct WindowInfo
	{
		int windowWidth;
		int windowHeight;
	};

	class Game
	{
	public:
		Game(GLFWwindow* window, const std::string& vertexPath, const std::string& fragmentPath, int winWidth, int winHeight);
		~Game();
		int init();
		void update();
		void processInput();
		void processMouse(double xpos, double ypos);
		void draw();

		void updateWinSize(int width, int height);
		WindowInfo windowInfo;
	private:
		GLFWwindow* window;

		Shader shaderProgram;
		Renderer renderer;
		glm::mat4 projection;
		Scene currentScene;

		float deltaTime = 0.0f;
		float lastFrame = 0.0f;

		bool cameraLocked = false;
	};
}