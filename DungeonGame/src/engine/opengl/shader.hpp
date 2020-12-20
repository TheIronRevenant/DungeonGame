#pragma once

#include <glm/glm.hpp>

#include <string>

namespace DG
{
	class Shader
	{
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		Shader(unsigned int id) : id{ id } {};
		void use();
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setMat4(const std::string& name, glm::mat4& value) const;

		unsigned int getId() const { return id;  }
	private:
		unsigned int id;
	};
}