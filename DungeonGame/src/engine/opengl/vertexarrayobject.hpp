#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace DG
{
	class VertexArrayObject
	{
	public:
		VertexArrayObject(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, unsigned int texture);
		void clear();
		void draw();
	private:
		unsigned int id;
		unsigned int bufferId;
		unsigned int elementId;
		unsigned int textureId;
		unsigned int indices;
	};
}