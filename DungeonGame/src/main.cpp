#include "engine/game.hpp"
#include "engine/logger.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main(void)
{
	//Logger initialization
	std::time_t creationTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char timeStr[26];
	ctime_s(timeStr, sizeof(timeStr), &creationTime);
	DG::log << "Output Log Created At " << timeStr << "\n";

	/*
	GLFW and OpenGL setup
	*/

	GLFWwindow* window;

	if (!glfwInit())
	{
		DG::log << "GLFW failed to init, dumped log\n";
		DG::dumpLog();
		return -1;
	} 
	else
	{
		DG::log << "GLFW initialized successfully\n";
	}

	//Size should adapt to screen size... eventually
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidMode;
	vidMode = glfwGetVideoMode(monitor);
	const int defaultWidth = vidMode->width / 1.92;
	const int defaultHeight = vidMode->height / 1.35;
	window = glfwCreateWindow(defaultWidth, defaultHeight, "Dungeon Game", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		DG::log << "Window failed to create, dumped log\n";
		DG::dumpLog();
		return -3;
	}
	else
	{
		DG::log << "Window created successfully\n";
	}

	{
		int monitorX;
		int monitorY;
		glfwGetMonitorPos(monitor, &monitorX, &monitorY);
		glfwSetWindowPos(window,
			monitorX + (vidMode->width - defaultWidth) / 2,
			monitorY + (vidMode->height - defaultHeight) / 2);
	}

	glfwMakeContextCurrent(window);
	glViewport(0, 0, defaultWidth, defaultHeight);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetCursorPosCallback(window, mouse_callback);

	//GLEW init
	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK)
	{
		std::cout << glewGetErrorString(glewErr) << std::endl;
		glfwTerminate();
		DG::log << "GLEW init error: " << glewGetErrorString(glewErr) << ", dumped log\n";
		DG::dumpLog();
		return -2;
	}
	else 
	{
		DG::log << "GLEW initialized successfully\n";
	}

	//Game holds a weak pointer, anything done with window in memory should be done here
	DG::Game game(window, "shaders\\vertex.vert", "shaders\\fragment.frag", defaultWidth, defaultHeight);
	glfwSetWindowUserPointer(window, &game);
	int returnCode = game.init();

	if (!returnCode)
	{
		DG::log << "Game exited successfully code " << returnCode << "\n";
	}
	else
	{
		DG::log << "Game closed unexpectedly code " << returnCode << "\n";
		DG::dumpLog();
	}

	glfwTerminate();

	return returnCode;
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	((DG::Game*)glfwGetWindowUserPointer(window))->updateWinSize(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	((DG::Game*)glfwGetWindowUserPointer(window))->processMouse(xpos, ypos);
}