#include "game.hpp"
#include "logger.hpp"
#include "gameobject.hpp"
#include "opengl/texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <algorithm>

DG::Game::Game(GLFWwindow* window, const std::string& vertexPath, const std::string& fragmentPath, int winWidth, int winHeight)
	: window{ window },
	shaderProgram{ vertexPath, fragmentPath },
	windowInfo{ winWidth, winHeight }
{
	//OpenGL config
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!cameraLocked)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	//Load textures
	textures["Default"] = loadOpaqueTexture("resources\\textures\\default.png");

	/*
	Test scene creation
	*/

	//Coords, Color, Texture
	std::vector<float> vertices = {
		0.5f, 0.5f, -20.0f,	1.0f, 1.0f, //Top Right
		0.5f, -0.5f, -20.0f,	1.0f, 0.0f, //Bottom Right
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //Bottom Left
		-0.5, 0.5f, 0.0f,	0.0f, 1.0f //Top Left
	};

	std::vector<unsigned int> indices = {
		0, 1, 3, //First Triangle
		1, 2, 3 //Second Triangle
	};

	std::vector<float> vertices2 = {
		0.25f, 0.5f, 0.0f,		1.0f, 1.0f, //Top Right
		0.25f, -0.5f, 0.0f,		1.0f, 0.0f, //Bottom Right
		-0.25f, -0.5f, 0.0f,	0.0f, 0.0f, //Bottom Left
		-0.25f, 0.5f, 0.0f,		0.0f, 1.0f //Top Left
	};

	std::vector<unsigned int> indices2 = {
		0, 1, 3, //First Triangle
		1, 2, 3 //Second Triangle
	};

	currentScene.opaqueObjects.emplace_back(vertices, indices, textures["Default"], shaderProgram.getId());
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.75f, 0.0f, 0.0f));
	currentScene.opaqueObjects.emplace_back(trans, vertices2, indices2, textures["Default"], shaderProgram.getId());
	currentScene.camera.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	projection = glm::perspective(glm::radians(45.0f), (float)windowInfo.windowWidth / (float)windowInfo.windowHeight, 0.1f, 100.f);

	shaderProgram.use();
	shaderProgram.setInt("tex", 0);
	shaderProgram.setMat4("projection", projection);
}

int DG::Game::init()
{
	//Partial lock-step update
	double timePerFrame = 1.L / 60.L;
	double lastUpdate = glfwGetTime();

	//Fps timer
	//Just steals from lastUpdate because it removed the need for a function call
	double fpsTime = lastUpdate;
	int updates = 0;

	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();

		while (lastUpdate < currentTime)
		{
			float updateTime = (float)glfwGetTime();
			deltaTime = updateTime - lastFrame;
			lastFrame = updateTime;

			update();
			draw();
			glfwPollEvents();

			lastUpdate += timePerFrame;

			updates++;
			if (currentTime - fpsTime >= 1.0L)
			{
				//This method of outputting fps is kind of bad but it works for now
				std::cout << "          " << "\r";
				std::cout << "FPS: " << updates << "\r";
				updates = 0;
				fpsTime = currentTime;
			}
		}
	}

	return 0;
}

void DG::Game::update()
{
	processInput();
}

void DG::Game::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (!cameraLocked)
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
			currentScene.camera.move(move, deltaTime);
	}
}

void DG::Game::processMouse(double xpos, double ypos)
{
	if (!cameraLocked)
	{
		currentScene.camera.processMouse(xpos, ypos);
	}
}

void DG::Game::draw()
{
	glClearColor(0.2f, 0.3f, 0.4f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer.draw(currentScene, shaderProgram);

	glfwSwapBuffers(window);
}

void DG::Game::updateWinSize(int width, int height)
{
	if (width > 0 && height > 0)
	{
		windowInfo.windowWidth = width;
		windowInfo.windowHeight = height;

		projection = glm::perspective(glm::radians(45.0f), (float)windowInfo.windowWidth / (float)windowInfo.windowHeight, 0.1f, 100.f);
		shaderProgram.setMat4("projection", projection);
	}
}

DG::Game::~Game()
{
	for (auto it = textures.begin(); it != textures.end(); it++)
	{
		glDeleteTextures(1, &it->second);
	}
	textures.clear();

	glDeleteProgram(shaderProgram.getId());
}