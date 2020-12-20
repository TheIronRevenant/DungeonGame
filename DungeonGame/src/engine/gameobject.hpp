#pragma once

#include "opengl/vertexarrayobject.hpp"

#include <glm/glm.hpp>

#include <vector>

namespace DG
{
	class GameObject
	{
	public:
		GameObject(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, unsigned int texture, unsigned int shaderId);
		GameObject(const glm::mat4& transform, const std::vector<float>& vertices, const std::vector<unsigned int>& indices, unsigned int texture, unsigned int shaderId);
		GameObject(const GameObject& other) : vao{ other.vao }, localVertices{ other.localVertices }, transform{ other.transform }, shaderId{ other.shaderId } {}
		GameObject(GameObject&& other) noexcept : vao{ other.vao }, localVertices{ other.localVertices }, transform{ other.transform }, shaderId{ other.shaderId } {}
		void setTransform(const glm::mat4& trans) { transform = trans; }
		void rotate(const float& degrees, const glm::vec3& axis);
		void translate(const glm::vec3& translation);
		void draw();
		void clear() { vao.clear(); }

		glm::vec3 getPosition() const { return glm::vec3{ transform[3] }; }

		GameObject& operator= (const GameObject& other)
		{
			vao = other.vao;
			localVertices = other.localVertices;
			transform = other.transform;
			shaderId = other.shaderId;
			return *this;
		}
		GameObject& operator= (GameObject&& other) noexcept
		{
			vao = other.vao;
			localVertices = other.localVertices;
			transform = other.transform;
			shaderId = other.shaderId;
			return *this;
		}
	private:
		VertexArrayObject vao;
		std::vector<glm::vec3> localVertices;
		glm::mat4 transform;
		unsigned int shaderId;
	};
}