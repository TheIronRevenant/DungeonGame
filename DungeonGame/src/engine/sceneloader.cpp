#include "sceneloader.hpp"
#include "scene.hpp"
#include "gameobject.hpp"
#include "opengl/texture.hpp"
#include "logger.hpp"
#include "opengl/shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>

void loadSection(std::string data, std::vector<DG::GameObject>& objects, const DG::Shader& shader);

void DG::loadScene(std::string file, Scene& scene, const Shader& shader)
{
	std::ifstream ifs(file);
	if (!ifs.good())
	{
		DG::log << "Failed to open file " << file;
		DG::dumpLog();
		exit(-6);
	}

	std::string data;
	ifs.seekg(0, std::ios::end);
	data.reserve(ifs.tellg());
	ifs.seekg(0, std::ios::beg);

	data.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

	data.erase(std::remove_if(data.begin(), data.end(), std::isspace));
	data.resize(data.find("exit", 0) + 4);

	std::size_t opaquepos = data.find("opaque{") + 7;
	std::string opaquedata = data.substr(opaquepos, data.find("}", opaquepos) - opaquepos);
	loadSection(opaquedata, scene.opaqueObjects, shader);
}

void loadSection(std::string data, std::vector<DG::GameObject>& objects, const DG::Shader& shader)
{
	std::stringstream datastream(data);
	std::string line;
	while (std::getline(datastream, line, '['))
	{
		std::string params;
		std::getline(datastream, params, ']');

		if (line == "beginquad")
		{
			std::vector<float> vertices;
			std::vector<unsigned int> indices;
			glm::vec3 translate(0.f);
			std::string texture;

			while (std::getline(datastream, line, '['))
			{
				std::getline(datastream, params, ']');

				if (line == "vertices")
				{
					std::stringstream vals(params);
					std::string v;
					while (std::getline(vals, v, ','))
					{
						vertices.push_back(std::stof(v));
					}
				}
				else if (line == "indices")
				{
					std::stringstream vals(params);
					std::string v;
					while (std::getline(vals, v, ','))
					{
						indices.push_back(std::stoul(v));
					}
				}
				else if (line == "translate")
				{
					std::stringstream vals(params);
					std::string v;

					std::getline(vals, v, ',');
					translate.x = std::stof(v);
					std::getline(vals, v, ',');
					translate.y = std::stof(v);
					std::getline(vals, v, ',');
					translate.z = std::stof(v);
				}
				else if (line == "texture")
				{
					texture = params;
				}
				else if (line == "end")
				{
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translate);
					objects.emplace_back(transform, vertices, indices, DG::textures[texture], shader.getId());
				}
			}
		}
	}
}