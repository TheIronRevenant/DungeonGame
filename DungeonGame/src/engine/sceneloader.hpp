#pragma once

#include <string>

namespace DG
{
	class Shader;
	struct Scene;

	void loadScene(std::string file, Scene& scene, const Shader& shader);
}