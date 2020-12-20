#include "vertexarrayobject.hpp"

#include <GL/glew.h>

DG::VertexArrayObject::VertexArrayObject(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, unsigned int texture)
	: textureId{ texture }, indices{ (unsigned int)indices.size() }
{
	//Initialize VAO and VBO
	glGenVertexArrays(1, &id);
	glGenBuffers(1, &bufferId);
	glGenBuffers(1, &elementId);

	glBindVertexArray(id);

	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	//Coordinate
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void DG::VertexArrayObject::draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glBindVertexArray(id);
	glDrawElements(GL_TRIANGLES, (int)indices, GL_UNSIGNED_INT, 0);
}

void DG::VertexArrayObject::clear()
{
	glDeleteVertexArrays(1, &id);
	glDeleteBuffers(1, &bufferId);
	glDeleteBuffers(1, &elementId);
}