#include "game.hpp"
#include "logger.hpp"
#include "gameobject.hpp"
#include "opengl/texture.hpp"
#include "sceneloader.hpp"

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
	DG::loadScene("resources\\scenes\\test.dat", currentScene, shaderProgram);
	currentScene.player.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

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
	currentScene.player.update(window, deltaTime);
	processInput();
}

void DG::Game::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void DG::Game::processMouse(double xpos, double ypos)
{
	if (!cameraLocked)
	{
		currentScene.player.camera.processMouse(xpos, ypos);
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