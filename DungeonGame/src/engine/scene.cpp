#include "scene.hpp"

DG::Scene::~Scene()
{
	for (auto& go : opaqueObjects)
	{
		go.clear();
	}

	for (auto& go : transparentObjects)
	{
		go.clear();
	}
}