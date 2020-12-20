#pragma once

#include "camera.hpp"
#include "gameobject.hpp"

#include <vector>

struct GLFWwindow;

namespace DG
{
	struct Scene
	{
		~Scene();

		std::vector<GameObject> opaqueObjects;
		std::vector<GameObject> transparentObjects;
		Camera camera;
	};
}