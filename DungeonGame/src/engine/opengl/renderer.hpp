#pragma once

namespace DG
{
	struct Scene;
	class Shader;

	class Renderer
	{
	public:
		void draw(Scene& scene, Shader& shader);
	};
}