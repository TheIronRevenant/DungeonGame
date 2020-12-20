#include "gameobject.hpp"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

DG::GameObject::GameObject(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, unsigned int texture, unsigned int shaderId)
	: vao{ vertices, indices, texture }, shaderId{ shaderId }
{
	for (const unsigned int& i : indices)
	{
		localVertices.emplace_back(vertices.at(0 + (i * 5ull)), vertices.at(1 + (i * 5ull)), vertices.at(2 + (i * 5ull)));
	}

	transform = glm::mat4(1.0f);
}

DG::GameObject::GameObject(const glm::mat4& transform, const std::vector<float>& vertices, const std::vector<unsigned int>& indices, unsigned int texture, unsigned int shaderId)
	: vao{ vertices, indices, texture }, shaderId{ shaderId }, transform{ transform }
{
	for (const unsigned int& i : indices)
	{
		localVertices.emplace_back(vertices.at(0 + (i * 5ull)), vertices.at(1 + (i * 5ull)), vertices.at(2 + (i * 5ull)));
	}
}

void DG::GameObject::rotate(const float& degrees, const glm::vec3& axis)
{
	transform = glm::rotate(transform, glm::radians(degrees), axis);
}

void DG::GameObject::translate(const glm::vec3& translation)
{
	transform = glm::translate(transform, translation);
}

void DG::GameObject::draw()
{
	glUseProgram(shaderId);
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(transform));

	vao.draw();
}