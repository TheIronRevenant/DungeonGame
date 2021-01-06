#include "renderer.hpp"
#include "../scene.hpp"
#include "shader.hpp"

#include <GL/glew.h>
#include <glm/gtx/norm.hpp>

#include <algorithm>

void DG::Renderer::draw(DG::Scene& scene, Shader& shader)
{
	glm::mat4 view = scene.player.camera.getViewMatrix();
	shader.setMat4("view", view);

	//Draw all opaque objects before transparent objects
	glDisable(GL_BLEND);
	for (GameObject& go : scene.opaqueObjects)
	{
		go.draw();
	}

	std::sort(scene.transparentObjects.begin(), scene.transparentObjects.end(),
		[&scene](const GameObject& a, const GameObject& b) -> bool {
			float aLength = glm::length2(scene.player.camera.getPosition() - a.getPosition());
			float bLength = glm::length2(scene.player.camera.getPosition() - b.getPosition());
			//Use the opposite because we want to render from farthest to nearest
			return aLength > bLength;
		});

	glEnable(GL_BLEND);
	for (GameObject& go : scene.transparentObjects)
	{
		go.draw();
	}
}